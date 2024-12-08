/**
 * @file lagrange_interpolator.hpp
 * @author Alexandre Hebert (alexandre.hebert@epfl.ch)
 * @brief 
 * @version 0.1
 * @date 2024-12-01
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#include "Eigen/Core"
#include "polynomial_interpolator.hpp"

#ifndef __LAGRANGE_INTERPOLATOR_INCLUDE
#define __LAGRANGE_INTERPOLATOR_INCLUDE

/**
 * @brief  Class implementing the Lagrange interpolation scheme
 *
 * @tparam T dataypes (int, float, double)
 */
template <typename T>
class LagrangeInterpolator: public PolynomialInterpolator<T> {

    private:
        /**
         * @brief Calculate basis value for datapoint i considering given 2D interpolation point x 
         * \f[
         *      l_i(x) = \prod_{j,j \neq i}^{n} \frac{x-x_j}{x_i-x_j}
         * \f]
         * @param i Index of the basis function (refers to datapoint x_i)  
         * @param x_interpolation The currently interpolated 2D datapoint x
         * @return T Value of the i-th polynomial evaluated @x
         */
        T lagrange_basis(unsigned int i, T x_interpolation);

        /**
         * @brief Generic implementation of lagrange_basis function for an m-dimensional datapoint
         * 
         * @param i Index of the basis function (refers to datapoint x_i) 
         * @param x_interpolation m-dimensional interpolation point x
         * @return T Value of the i-th polynomial evaluated @x
         */
        T lagrange_basis(unsigned int i, const Eigen::VectorX<T>& x_interpolation);

    public:
        /**
         * @brief Constructs a Lagrange interpolator object
         */
        LagrangeInterpolator();

        Eigen::VectorX<T> operator()(const Eigen::MatrixX<T>& X) override;
};

#endif