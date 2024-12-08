/**
 * @file chebychev_interpolator.hpp
 * @author Alexandre Hebert (alexandre.hebert@epfl.ch)
 * @brief 
 * @version 0.1
 * @date 2024-12-08
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#include "polynomial_interpolator.hpp"
#include "Eigen/Core"

#ifndef __CHEBYCHEV_INTERPOLATOR_INCLUDE
#define __CHEBYCHEV_INTERPOLATOR_INCLUDE

template <typename T>
class ChebychevInterpolator: public PolynomialInterpolator<T>
{
    public:
        enum SamplingType {
            UNKNOWN,
            TYPE_1,
            TYPE_2
        };

    private:
        Eigen::VectorX<T> w;

        SamplingType sampling_type;

        SamplingType detect_sampling();

    public:

        ChebychevInterpolator();

        ChebychevInterpolator(SamplingType sampling);

        void fit(const Eigen::MatrixX<T>& X, unsigned int dim_idx) override;

        void fit(const Eigen::MatrixX<T>& X, const Eigen::VectorX<T>& y) override;

        Eigen::VectorX<T> operator()(const Eigen::MatrixX<T>& X) override;

};

#endif

