/**
 * @file Interpolator.hpp
 * @author Alexandre Hebert (alexandre.hebert@epfl.ch)
 * @brief Interpolator abstract class definition
 * @version 0.1
 * @date 2024-11-28
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#ifndef __INTERPOLATOR_INCLUDE
#define __INTERPOLATOR_INCLUDE

#include <Eigen/Core>


/**
 * @brief Interpolator abstract class definition contains necessary 
 * functions & operators for defining an interpolator that uses 
 * datapoints.
 * 
 * @tparam T Interpolation points dataype to be used (int, float, double)
 */
template <typename T>
class Interpolator {

    public:

        /**
         * @brief Fits an interpolator model from the provided datapoints
         * 
         *
         * @param X Matrix of N datapoints of dimension M+1 (includes interpolated dim)
         * @param dim_idx the column idx to be used as the interpolated dimension
         */
        virtual void fit(const Eigen::MatrixX<T>& X, unsigned int dim_idx) = 0;

        /**
         * @brief Fits an interpolator model from the provided datapoints
         * 
         * @param X Matrix of N datapoints of dimension M
         * @param y Vector of N datapoints to be fitted by the interpolator
         */
        virtual void fit(const Eigen::MatrixX<T>& X, const Eigen::VectorX<T>& y) = 0;

        /**
         * @brief Interpolate the given N datapoints of matrix X with the fitted model
         * 
         * @param X Datapoints to interpolated
         * @return Eigen::VectorX<T> Interpolated points
         */
        virtual Eigen::VectorX<T> operator()(const Eigen::MatrixX<T>& X) = 0;

        /**
         * @brief Interpolate the single 1D datapoint given
         * 
         * @param x Datapoint to be interpolated
         * @return T Interpolated point
         */
        virtual T operator()(T x) = 0;
};

#endif