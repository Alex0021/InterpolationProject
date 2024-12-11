#pragma once

#include <unsupported/Eigen/FFT>
#include <Eigen/Core>
#include "project_exceptions.hpp"

template <typename T>
class FourierApproximator {

    Eigen::VectorX<std::complex<T>> _freq;
    Eigen::FFT<T> _fft;
    T _x_min, _x_max;

    public:
        FourierApproximator();
        void fit(const Eigen::VectorX<T>& v);
        Eigen::VectorX<T> approximate_points(int n);

};