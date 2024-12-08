#include "chebychev_interpolator.hpp"

template <typename T>
ChebychevInterpolator<T>::ChebychevInterpolator():
    sampling_type(SamplingType::UNKNOWN)     
{}

template <typename T>
ChebychevInterpolator<T>::ChebychevInterpolator(SamplingType sampling):
    sampling_type(sampling)     
{}

template <typename T>
void ChebychevInterpolator<T>::fit(const Eigen::MatrixX<T>& X, unsigned int dim_idx) {}

template <typename T>
void ChebychevInterpolator<T>::fit(const Eigen::MatrixX<T>& X,  const Eigen::VectorX<T>& y) {}

template <typename T>
Eigen::VectorX<T> ChebychevInterpolator<T>::operator()(const Eigen::MatrixX<T>& X) {}

template <typename T>
ChebychevInterpolator<T>::SamplingType ChebychevInterpolator<T>::detect_sampling() {}

template class ChebychevInterpolator<int>;
template class ChebychevInterpolator<float>;
template class ChebychevInterpolator<double>;