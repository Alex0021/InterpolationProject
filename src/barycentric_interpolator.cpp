#include "barycentric_interpolator.hpp"
#include "exception"

template <typename T>
BarycentricInterpolator<T>::BarycentricInterpolator() {}

template <typename T>
T BarycentricInterpolator<T>::barycentric_weight(unsigned int i, unsigned int j) 
{
    throw std::runtime_error("FUNCTION NOT YET IMPLEMENTED ==> barycentric_weight");
}

template <typename T>
void BarycentricInterpolator<T>::add_data(Eigen::MatrixX<T> X)
{
    throw std::runtime_error("FUNCTION NOT YET IMPLEMENTED ==> barycentric::add_data");
}

template <typename T>
void BarycentricInterpolator<T>::fit(const Eigen::MatrixX<T>& X, unsigned int dim) {}

template <typename T>
void BarycentricInterpolator<T>::fit(const Eigen::MatrixX<T>& X, const Eigen::VectorX<T>& y) {}

template <typename T>
Eigen::VectorX<T> BarycentricInterpolator<T>::operator()(const Eigen::MatrixX<T>& X) 
{
    throw std::runtime_error("FUNCTION NOT YET IMPLEMENTED ==> barycentric::operator");
}

template class BarycentricInterpolator<int>;
template class BarycentricInterpolator<float>;
template class BarycentricInterpolator<double>;