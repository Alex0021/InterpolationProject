/**
 * @file polynomial_interpolator.hpp
 * @author Alexandre Hebert (alexandre.hebert@epfl.ch)
 * @brief Abstract Class PolynomialInterpolator defining requirements for polynomial interpolation
 * @version 0.1
 * @date 2024-11-28
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#ifndef __POLYNOMIAL_INTERPOLATOR_INCLUDE
#define __POLYNOMIAL_INTERPOLATOR_INCLUDE

#include "interpolator.hpp"

/**
 * @brief Class for polynomial interpolator models
 * 
 *
 * @tparam T Datatype to be used (int, float, double)
 */
template <typename T>
class PolynomialInterpolator: public Interpolator<T> {

    protected:
        /* Matrix of NxM datapoints */
        Eigen::MatrixX<T> _X_data;
        /* Vector of Nx1 datapoitns for fitting */
        Eigen::VectorX<T> _y_data;

    public: 
        /**
         * @brief Getter for the X datapoints matrix 
         *
         * @return X matrix for the N datapoints of the interpolator
         */
        const Eigen::MatrixX<T>& get_X_data() const;

        /**
         * @brief Getter for the y datapoints vector
         * @return y vector of the interpolator
         */
        const Eigen::VectorX<T>& get_y_data() const;

        /**
         * @brief Construct a new Polynomial Interpolator object
         */
        PolynomialInterpolator();

        void fit(const Eigen::MatrixX<T>& X, unsigned int dim_idx) override;

        void fit(const Eigen::MatrixX<T>& X, const Eigen::VectorX<T>& y) override;

        Eigen::VectorX<T> operator()(const Eigen::MatrixX<T>& X) override;
};

#endif