#ifndef __CUBIC_SPLINE_INTERPOLATOR_INCLUDE
#define __CUBIC_SPLINE_INTERPOLATOR_INCLUDE

#include "polynomial_interpolator.hpp"
#include "project_exceptions.hpp"
#include <iostream>
#include <Eigen/LU>


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
         * @brief Boundary condition of the current spline instance
         */
        BoundaryConstraint boundary_constraint;

        Eigen::MatrixX<T> X_data;

        Eigen::MatrixX4<T> coefficients; // Coefficients of the cubic spline (a,b,c,d)

        Eigen::Vector2<T> clamped_values;

        void _apply_boundary_conditions(Eigen::MatrixX<T> &A, Eigen::VectorX<T> &b, Eigen::VectorX<T> &f, Eigen::VectorX<T> &h);

        int _get_index(T x);

    public:

        CubicSplineInterpolator();

        CubicSplineInterpolator(BoundaryConstraint boundary);

        CubicSplineInterpolator(BoundaryConstraint boundary, const Eigen::Vector2<T>& clamped_values);

        void fit(const Eigen::MatrixX<T>& X, unsigned int dim_idx) override;

        void fit(const Eigen::VectorX<T>& X, const Eigen::VectorX<T>& y);

        Eigen::VectorX<T> operator()(const Eigen::MatrixX<T>& X) override;

        T operator()(T x) override;

        void set_clamped_values(const Eigen::Vector2<T>& clamped_values);
        
        void set_clamped_values(T low, T high);

};

#endif