#include "cubic_spline_interpolator.hpp"

template <typename T>
CubicSplineInterpolator<T>::CubicSplineInterpolator() {}

template <typename T>
CubicSplineInterpolator<T>::CubicSplineInterpolator(BoundaryConstraint boundary):
    boundary_constraint(boundary)
{}

template <typename T>
void CubicSplineInterpolator<T>::fit(const Eigen::MatrixX<T>& X, unsigned int dim_idx) {}

template <typename T>
void CubicSplineInterpolator<T>::fit(const Eigen::MatrixX<T>& X, const Eigen::VectorX<T>& y) {}

template <typename T>
void CubicSplineInterpolator<T>::fit(const Eigen::MatrixX<T>& X, const Eigen::VectorX<T>& y, BoundaryConstraint boundary) {} 

template <typename T>
Eigen::VectorX<T> CubicSplineInterpolator<T>::operator()(const Eigen::MatrixX<T>& X) {}

template class CubicSplineInterpolator<int>;
template class CubicSplineInterpolator<double>;
template class CubicSplineInterpolator<float>;
