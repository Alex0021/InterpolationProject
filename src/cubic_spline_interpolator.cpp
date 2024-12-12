#include "cubic_spline_interpolator.hpp"

template <typename T>
CubicSplineInterpolator<T>::CubicSplineInterpolator() {
    if (std::is_same<T, int>::value) {
        throw CubicSplineInterpolatorException::InvalidType("CubicSplineInterpolator doesn't support int type", __func__);
    }
    boundary_constraint = BoundaryConstraint::NATURAL;
}

template <typename T>
CubicSplineInterpolator<T>::CubicSplineInterpolator(BoundaryConstraint boundary){
    if (boundary == BoundaryConstraint::CLAMPED) {
        std::cerr << "Warning: Clamped boundary condition requires boundary values and none were provided. Using 0,0. Can set them using set_clamped_values" << std::endl;
        this->clamped_values = Eigen::Vector2<T>(0, 0);
    }
    CubicSplineInterpolator();
    boundary_constraint = boundary;
}

template <typename T>
CubicSplineInterpolator<T>::CubicSplineInterpolator(BoundaryConstraint boundary, const Eigen::Vector2<T>& clamped_values){ 
    
    if (boundary != BoundaryConstraint::CLAMPED) {
        std::cerr << "Warning: Non-clamped boundary condition provided with clamped values. Ignoring clamped values." << std::endl;
    } else {
        this->clamped_values = clamped_values;
    }
    CubicSplineInterpolator();
    boundary_constraint = boundary;
}

template <typename T>
void CubicSplineInterpolator<T>::_apply_boundary_conditions(Eigen::MatrixX<T> &A, Eigen::VectorX<T> &b, Eigen::VectorX<T> &f, Eigen::VectorX<T> &h) 
{
    int n = A.rows();
    switch (boundary_constraint) {
        case BoundaryConstraint::NATURAL:
            A(0, 0) = 1;
            A(n - 1, n - 1) = 1;
            b(0) = 0;
            b(n - 1) = 0;
            break;
        case BoundaryConstraint::CLAMPED:
            A(0, 0) = 2*h(0);
            A(0, 1) = h(0);
            A(1, 0) = h(0);
            A(n - 1, n - 1) = 2*h(n - 2);
            A(n - 1, n - 2) = h(n - 2);
            A(n - 2, n - 1) = h(n - 2);
            b(0) = 3*(f(0) - clamped_values(0));
            b(n - 1) = 3*(clamped_values(1) - f(n - 2));
            break;
        case BoundaryConstraint::NOT_A_KNOT:

        default:
            throw CubicSplineInterpolatorException::InvalidType("Invalid boundary condition", __func__);
    }
}

template <typename T>
void CubicSplineInterpolator<T>::fit(const Eigen::MatrixX<T>& X, unsigned int dim_idx) 
{
    int n, m;
    n = X.rows();
    m = X.cols();
    if (m > 2) {
        throw CubicSplineInterpolatorException::MultidimensionalImplementation("Multidimensional data not supported", __func__);
    }

    if (dim_idx >= m) {
        throw CubicSplineInterpolatorException::IndexOutOfBounds(__func__);
    }

    if (m == 1) {
        throw CubicSplineInterpolatorException::InterpolationProjectException("Single dimension data not supported", __func__);
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

    this->_apply_boundary_conditions(A, v, f, h);

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
int CubicSplineInterpolator<T>::_get_index(T x) 
{
    
    T xmin = this->_X_min(0);
    T xmax = this->_X_max(0);

    if (x < xmin || x > xmax) {
        throw CubicSplineInterpolatorException::Extrapolation(x, xmin, xmax, __func__);
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
Eigen::VectorX<T> CubicSplineInterpolator<T>::operator()(const Eigen::MatrixX<T>& X) 
{
    if (X.cols() > 1) {
        throw CubicSplineInterpolatorException::MultidimensionalImplementation("Multidimensional data not supported", __func__);
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

template <typename T>
void CubicSplineInterpolator<T>::set_clamped_values(const Eigen::Vector2<T>& values) {
    this->clamped_values = values;
}

template <typename T>
void CubicSplineInterpolator<T>::set_clamped_values(T lower, T upper) {
    this->clamped_values = Eigen::Vector2<T>(lower, upper);
}

template <typename T>
CubicSplineInterpolator<T>::BoundaryConstraint CubicSplineInterpolator<T>::get_constraint_from_string(const std::string& b_constraint)
{
    const auto to_uppercase = [](std::string& s) {return std::transform(s.begin(), s.end(), s.begin(),
                                                                        [](char c) {return std::toupper(c); }); };
    std::string b_const_normalized(b_constraint);
    to_uppercase(b_const_normalized);
    if (b_const_normalized == "NATURAL") { return CubicSplineInterpolator<T>::BoundaryConstraint::NATURAL; }
    if (b_const_normalized == "NOT_A_KNOT") { return CubicSplineInterpolator<T>::BoundaryConstraint::NOT_A_KNOT; }
    if (b_const_normalized == "CLAMPED") { return CubicSplineInterpolator<T>::BoundaryConstraint::CLAMPED; }
    else { throw CubicSplineInterpolatorException::InvalidType("Cannot convert to a known boundary condition!", __func__);}
}

// template class CubicSplineInterpolator<int>;
template class CubicSplineInterpolator<double>;
template class CubicSplineInterpolator<float>;
// template class CubicSplineInterpolator<int>;
