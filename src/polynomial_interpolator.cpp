#include "polynomial_interpolator.hpp"


template <typename T>
PolynomialInterpolator<T>::PolynomialInterpolator() {}

template <typename T>
void PolynomialInterpolator<T>::fit(const Eigen::MatrixX<T>& X, unsigned int dim_idx)
{
    this->_X_data = Eigen::MatrixX<T>(X.rows(), X.cols()-1);
    this->_X_data << X.leftCols(dim_idx), X.rightCols(X.cols()-dim_idx-1);
    this->_y_data = X.col(dim_idx);
}

template <typename T>
void PolynomialInterpolator<T>::fit(const Eigen::MatrixX<T>& X, const Eigen::VectorX<T>& y)
{
    this->_X_data = X;
    this->_y_data = y;
}

template <typename T>
Eigen::VectorX<T> PolynomialInterpolator<T>::operator()(const Eigen::MatrixX<T>& X) 
{
    return Eigen::Vector2<T>();
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
