/**
 * @file cubic_spline_interpolator.hpp
 * @author Max Brodeur (maxbrod21@gmail.com)
 * @brief A class for cubic spline interpolation
 * @version 0.1
 * @date 2024-12-12
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#ifndef __CUBIC_SPLINE_INTERPOLATOR_INCLUDE
#define __CUBIC_SPLINE_INTERPOLATOR_INCLUDE

#include "polynomial_interpolator.hpp"
#include "project_exceptions.hpp"
#include <iostream>
#include <Eigen/LU>

/**
 * @brief Templated class for cubic spline interpolation
 * 
 * @tparam T 
 */
template <typename T>
class CubicSplineInterpolator: public PolynomialInterpolator<T>
{
    public:
        /**
         * @brief Specify the condition on the end nodes of the spline
         */
        enum BoundaryConstraint {
            NATURAL,
            NOT_A_KNOT,
            CLAMPED,
        };
    
    private:
        /**
         * @brief Boundary condition to apply to the spline interpolation
         */
        BoundaryConstraint boundary_constraint;

        /**
         * @brief Coefficients of the cubic spline interpolation 
         * The cubic polynomial for the i-th segment represented as:
         * \f[
         * S_i(x) = a_i + b_i (x - x_i) + c_i (x - x_i)^2 + d_i (x - x_i)^3
         * \f]
         * where a_i, b_i, c_i, and d_i are the coefficients for the i-th cubic segment.
         */
        Eigen::MatrixX4<T> coefficients; 

        /**
         * @brief The values to clamp the spline to if the boundary condition is set to CLAMPED
         */
        Eigen::Vector2<T> clamped_values;

        /**
         * @brief Private function for applying the boundary conditions for the trilinear system matrix A and the vector b. 
         * Ex: NATURAL boundary conditions impose A_{11}=A_{nn}=1 and b_1=b_n=0
         * 
         * @param A : The trilinear system matrix
         * @param b : The trilinear system vector
         * @param f : Newton's divided differences `f[i] = (y[i+1] - y[i]) / h[i]\`
         * @param h : The datapoint difference vector  `h[i] = x[i+1] - x[i]`
         */
        void _apply_boundary_conditions(Eigen::MatrixX<T> &A, Eigen::VectorX<T> &b, Eigen::VectorX<T> &f, Eigen::VectorX<T> &h);

        /**
         * @brief A private method to identify the index of the segment (or 'bin') that contains the value x.
         * 
         * @param x : The value to find the segment for
         * @return int : The index of the bin
         */
        int _get_index(T x);

    public:

        /**
         * @brief Construct a new Cubic Spline Interpolator object. Defaults to NATURAL boundary conditions.
         * 
         */
        CubicSplineInterpolator();

        /**
         * @brief Construct a new Cubic Spline Interpolator object with the specified boundary conditions
         * 
         * @param boundary : boundary condition to apply to the spline
         */
        CubicSplineInterpolator(BoundaryConstraint boundary);

        /**
         * @brief Construct a new Cubic Spline Interpolator object for clamped boundary conditions with the specified clamped values
         * 
         * @param boundary : must be set to CLAMPED to use clamped values
         * @param clamped_values : The values to clamp the spline to
         */
        CubicSplineInterpolator(BoundaryConstraint boundary, const Eigen::Vector2<T>& clamped_values);

        /**
         * @brief Utility wrapper over the fit method to fit the data to the specified dimension index
         * 
         * @param X 
         * @param dim_idx 
         */
        void fit(const Eigen::MatrixX<T>& X, unsigned int dim_idx) override;

        /**
         * @brief Fit the data to the cubic spline interpolator using the specified X and y data. 
         * Computes the coefficients for the cubic spline by solving a trilinear system of equations.
         * 
         * @param X : The datapoints to fit the spline to 
         * @param y : The values of the datapoints to fit the spline to
         */
        void fit(const Eigen::VectorX<T>& X, const Eigen::VectorX<T>& y);

        /**
         * @brief Query the cubic spline interpolator for the interpolated values at the specified datapoints X
         * 
         * @param X : The datapoints to query the interpolator for
         * @return Eigen::VectorX<T> : The interpolated values at the specified datapoints
         */
        Eigen::VectorX<T> operator()(const Eigen::MatrixX<T>& X) override;

        /**
         * @brief Query the cubic spline interpolator for the interpolated value at the specified datapoint x
         * 
         * @param x : The datapoint to query the interpolator for
         * @return T : The interpolated value at the specified datapoint
         */
        T operator()(T x) override;

        /**
         * @brief Set the clamped values object for CLAMPED boundary conditions
         * 
         * @param clamped_values 
         */
        void set_clamped_values(const Eigen::Vector2<T>& clamped_values);
        
        /**
         * @brief Set the clamped values object for CLAMPED boundary conditions
         * 
         * @param low 
         * @param high 
         */
        void set_clamped_values(T low, T high);

};

#endif