#include "polynomial_interpolator.hpp"
#include "project_exceptions.hpp"


template <typename T>
PolynomialInterpolator<T>::PolynomialInterpolator() {}

template <typename T>
void PolynomialInterpolator<T>::fit(const Eigen::MatrixX<T>& X, unsigned int dim_idx)
{
    // Check index limits
    if (dim_idx >= X.cols()) 
    {
        throw PolynomialInterpolatorException::IndexOutOfBounds(dim_idx, X.cols()-1);
    }
    this->_X_data = Eigen::MatrixX<T>(X.rows(), X.cols()-1);
    this->_X_data << X.leftCols(dim_idx), X.rightCols(X.cols()-dim_idx-1);
    this->_y_data = X.col(dim_idx);
}

template <typename T>
void PolynomialInterpolator<T>::fit(const Eigen::MatrixX<T>& X, const Eigen::VectorX<T>& y)
{
    // Check if # rows in X matches number of rows in y (=> # datapoints)
    if (X.rows() != y.rows())
    {
        throw PolynomialInterpolatorException::SizeMismatch(y.rows(), X.rows());
    }
    this->_X_data = X;
    this->_y_data = y;
}

/* GETTERS */
template <typename T>
const Eigen::MatrixX<T>& PolynomialInterpolator<T>::get_X_data() const 
{
    return this->_X_data;
}

template <typename T>
const Eigen::VectorX<T>& PolynomialInterpolator<T>::get_y_data() const 
{
    return this->_y_data;
}


template class PolynomialInterpolator<int>;
template class PolynomialInterpolator<float>;
template class PolynomialInterpolator<double>;
