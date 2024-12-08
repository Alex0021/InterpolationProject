#include "cubic_spline_interpolator.hpp"

template <typename T>
CubicSplineInterpolator<T>::CubicSplineInterpolator() {}

template <typename T>
CubicSplineInterpolator<T>::CubicSplineInterpolator(BoundaryConstraint boundary):
    boundary_constraint(boundary)
{}

template <typename T>
void CubicSplineInterpolator<T>::fit(const Eigen::MatrixX<T>& X, unsigned int dim_idx) 
{
    throw std::runtime_error("FUNCTION NOT YET IMPLEMENTED ==> cublic_spline::fit");
}

template <typename T>
void CubicSplineInterpolator<T>::fit(const Eigen::MatrixX<T>& X, const Eigen::VectorX<T>& y) 
{
    throw std::runtime_error("FUNCTION NOT YET IMPLEMENTED ==> cublic_spline::fit");
}

template <typename T>
void CubicSplineInterpolator<T>::fit(const Eigen::MatrixX<T>& X, const Eigen::VectorX<T>& y, BoundaryConstraint boundary) 
{
    throw std::runtime_error("FUNCTION NOT YET IMPLEMENTED ==> cublic_spline::fit");
} 

template <typename T>
Eigen::VectorX<T> CubicSplineInterpolator<T>::operator()(const Eigen::MatrixX<T>& X) 
{
    throw std::runtime_error("FUNCTION NOT YET IMPLEMENTED ==> cublic_spline::operator");
}

template class CubicSplineInterpolator<int>;
template class CubicSplineInterpolator<double>;
template class CubicSplineInterpolator<float>;
