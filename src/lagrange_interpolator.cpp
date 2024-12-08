#include "lagrange_interpolator.hpp"

template <typename T>
T LagrangeInterpolator<T>::lagrange_basis(unsigned int i, const Eigen::VectorX<T>& x_interpolation) 
{
    return 0;
}

template <typename T>
LagrangeInterpolator<T>::LagrangeInterpolator() {}

template <typename T>
Eigen::VectorX<T> LagrangeInterpolator<T>::operator()(const Eigen::MatrixX<T>& X) 
{
    throw std::runtime_error("FUNCTION NOT YET IMPLEMENTED ==> lagrange::operator");
}

template class LagrangeInterpolator<int>;
template class LagrangeInterpolator<float>;
template class LagrangeInterpolator<double>;
