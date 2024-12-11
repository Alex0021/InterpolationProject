#include "cubic_spline_interpolator.hpp"
#include <iostream>

template <typename T>
int CubicSplineInterpolator<T>::_get_index(T x) 
{
    
    T xmin = this->_X_min(0);
    T xmax = this->_X_max(0);

    if (x < xmin || x > xmax) {
        throw std::runtime_error("Value out of range");
    }

    int n = this->_X_data.rows();
    int guess = (int) (x - xmin) / (xmax - xmin) * n;
    
    auto is_in_bin = [&](int idx) {return this->_X_data(idx) <= x && x < this->_X_data(idx + 1);};

    int idx = guess;

    while (idx < n-1 && idx > 0 && !is_in_bin(idx)) {
        if (x < this->_X_data(idx)) {
            idx--;
        } else {
            idx++;
        }
    }
    idx = idx < 0 ? 0 : idx;
    idx = idx >= n-1 ? n-2 : idx;
    return idx;

}

template <typename T>
CubicSplineInterpolator<T>::CubicSplineInterpolator() {
    if (std::is_same<T, int>::value) {
        throw std::runtime_error("Invalid datatype for CubicSplineInterpolator");
    }
    boundary_constraint = BoundaryConstraint::NATURAL;
}

template <typename T>
CubicSplineInterpolator<T>::CubicSplineInterpolator(BoundaryConstraint boundary){
    CubicSplineInterpolator();
    boundary_constraint = boundary;
}

template <typename T>
void CubicSplineInterpolator<T>::_apply_boundary_conditions(Eigen::MatrixX<T> &A, Eigen::VectorX<T> &b) 
{
    int n = A.rows();
    switch (boundary_constraint) {
        case CubicSplineInterpolator<T>::BoundaryConstraint::NATURAL:
            A(0, 0) = 1;
            A(n - 1, n - 1) = 1;
            b(0) = 0;
            b(n - 1) = 0;
            break;
        default:
            throw std::runtime_error("Invalid boundary condition");
    }
}

template <typename T>
void CubicSplineInterpolator<T>::fit(const Eigen::MatrixX<T>& X, unsigned int dim_idx) 
{
    int n, m;
    n = X.rows();
    m = X.cols();
    if (m > 2) {
        throw std::runtime_error("Multidimensional data not supported");
    }

    if (dim_idx >= m) {
        throw std::runtime_error("Invalid dimension index");
    }

    if (m == 1) {
        throw std::runtime_error("Single dimensional data not supported");
    }

    Eigen::VectorX<T> y = X.col(dim_idx);
    int other_dim = ! (bool) dim_idx;
    Eigen::VectorX<T> x = X.col(other_dim);
    this->fit(x, y);
}

template <typename T>
void CubicSplineInterpolator<T>::fit(const Eigen::VectorX<T>& X, const Eigen::VectorX<T>& y) 
{
    this->_X_max = Eigen::VectorX<T>(1); // TODO: code for multi dim data
    this->_X_min = Eigen::VectorX<T>(1);

    this->_X_data = X;
    this->_y_data = y;

    this->_X_max << X.maxCoeff();
    this->_X_min << X.minCoeff();

    int n = X.rows();
    Eigen::MatrixX<T> A = Eigen::MatrixX<T>::Zero(n, n);
    Eigen::VectorX<T> v = Eigen::VectorX<T>::Zero(n);

    // diff: h[i] = x[i+1] - x[i]
    Eigen::VectorX<T> h = X.tail(n-1) - X.head(n-1);

    // Newton divided diff: f[i] = (y[i+1] - y[i]) / h[i]
    Eigen::VectorX<T> f = (y.tail(n-1) - y.head(n-1)).cwiseQuotient(h);

    this->_apply_boundary_conditions(A, v);

    // build A 
    Eigen::MatrixX<T> inner_A = A.block(1, 1, n - 2, n - 2);
    inner_A.diagonal(0)  = 2*(h.head(n-2) + h.tail(n-2));
    inner_A.diagonal(1)  = h.segment(1, n-3);
    inner_A.diagonal(-1) = h.segment(1, n-3);
    // insert back into A
    A.block(1, 1, n - 2, n - 2) = inner_A; 

    // build b
    v.segment(1, n - 2) = 3*(f.tail(n-2) - f.head(n-2));


    // solve for coefficients
    Eigen::VectorX<T> c = A.fullPivLu().solve(v);
    Eigen::VectorX<T> d = (c.tail(n-1) - c.head(n-1)).cwiseQuotient(3*h);
    Eigen::VectorX<T> b = f - h.cwiseProduct(2*c.head(n-1) + c.tail(n-1))/3;
    Eigen::VectorX<T> a = y.head(n-1);
    this->coefficients = Eigen::MatrixX4<T>(n-1, 4);
    this->coefficients << a, b, c.head(n-1), d;

}

template <typename T>
Eigen::VectorX<T> CubicSplineInterpolator<T>::operator()(const Eigen::MatrixX<T>& X) 
{
    if (X.cols() > 1) {
        throw std::runtime_error("Multidimensional data not supported");
    }

    Eigen::VectorX<T> y = Eigen::VectorX<T>(X.rows());
    for (int i = 0; i < X.rows(); i++) {
        y(i) = this->operator()(X(i));
    }
    return y;
}

template <typename T>
T CubicSplineInterpolator<T>::operator()(T x) 
{
    int i = this->_get_index(x);
    Eigen::Vector4<T> coeffs = this->coefficients.row(i);
    T dx = x - this->_X_data(i);
    return coeffs(0) + coeffs(1)*dx + coeffs(2)*dx*dx + coeffs(3)*dx*dx*dx;
}

// template class CubicSplineInterpolator<int>;
template class CubicSplineInterpolator<double>;
template class CubicSplineInterpolator<float>;
