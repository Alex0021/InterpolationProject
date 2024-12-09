#include "polynomial_interpolator.hpp"
#include "project_exceptions.hpp"
#include <tuple>


template <typename T>
PolynomialInterpolator<T>::PolynomialInterpolator() {}

template <typename T>
Eigen::VectorX<T> PolynomialInterpolator<T>::operator()(const Eigen::MatrixX<T>& X) 
{
    throw std::runtime_error("Operator() for PolynomialInterpolator baseclass should never be called!");
}

template <typename T>
Eigen::VectorX<T> PolynomialInterpolator<T>::operator()(const Eigen::VectorX<T>& x) 
{
    throw std::runtime_error("Operator() for PolynomialInterpolator baseclass should never be called!");
}

template <typename T>
T PolynomialInterpolator<T>::operator()(T x) 
{
    throw std::runtime_error("Operator() for PolynomialInterpolator baseclass should never be called!");
}

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

    // Update data range
    this->calculate_range();
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

    // Update data range
    this->calculate_range();
}

template <typename T>
void PolynomialInterpolator<T>::calculate_range()
{
    this->_X_max = Eigen::VectorX<T>(this->_X_data.cols());
    this->_X_min = Eigen::VectorX<T>(this->_X_data.cols());
    for (unsigned int i=0; i<this->_X_data.cols(); i++)
    {
        this->_X_max(i) = this->_X_data.col(i).maxCoeff();
        this->_X_min(i) = this->_X_data.col(i).minCoeff();
    }
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

template <typename T>
std::tuple<Eigen::VectorX<T>, Eigen::VectorX<T>> PolynomialInterpolator<T>::get_range() const
{
    return std::make_tuple(this->_X_min, this->_X_max);
}

template class PolynomialInterpolator<int>;
template class PolynomialInterpolator<float>;
template class PolynomialInterpolator<double>;
