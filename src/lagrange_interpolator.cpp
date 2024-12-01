#include "lagrange_interpolator.hpp"

template <typename T>
T LagrangeInterpolator<T>::lagrange_basis(unsigned int i, const Eigen::VectorX<T>& x_interpolation) {}

template <typename T>
T LagrangeInterpolator<T>::lagrange_basis(unsigned int i, T x_interpolation) {}

template <typename T>
LagrangeInterpolator<T>::LagrangeInterpolator() {}

template <typename T>
Eigen::VectorX<T> LagrangeInterpolator<T>::operator()(const Eigen::MatrixX<T>& X) {}

template class LagrangeInterpolator<int>;
template class LagrangeInterpolator<float>;
template class LagrangeInterpolator<double>;
