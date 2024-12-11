#include "fourier_approximator.hpp"

template <typename T>
FourierApproximator<T>::FourierApproximator() {
    if(std::is_same<T, int>::value) {
        throw InterpolationProjectException::InvalidType("FourierApproximator does not support int type!", __func__);
    }
}

template <typename T>
void FourierApproximator<T>::fit(const Eigen::VectorX<T> &v) {
    this->_freq = Eigen::VectorX<std::complex<T>>(v.rows());
    this->_x_min = v.minCoeff();
    this->_x_max = v.maxCoeff();
    this->_fft.fwd(this->_freq, v);
}

template <typename T>
Eigen::VectorX<T> FourierApproximator<T>::approximate_points(int n) {
    
    Eigen::FFT<T> fft;
    Eigen::VectorX<T> inv(n);
    fft.inv(inv, this->_freq, n);
    return inv;
}

template class FourierApproximator<double>;
template class FourierApproximator<float>;