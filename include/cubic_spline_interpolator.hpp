#ifndef __CUBIC_SPLINE_INTERPOLATOR_INCLUDE
#define __CUBIC_SPLINE_INTERPOLATOR_INCLUDE

#include "interpolator.hpp"

template <typename T>
class CubicSplineInterpolator: public Interpolator<T>
{
    public:
        /**
         * @brief Specify the condition on the end nodes of the spline
         */
        enum BoundaryConstraint {
            NATURAL,
            NOT_A_KNOT,
            PERIODIC,
            QUADRATIC
        };
    
    private:
        /**
         * @brief Boundary condition of the current spline instance
         */
        BoundaryConstraint boundary_constraint;

        Eigen::MatrixX<T> X_data;

        Eigen::VectorXd coefficients;

    public:

        CubicSplineInterpolator();

        CubicSplineInterpolator(BoundaryConstraint boundary);

        void fit(const Eigen::MatrixX<T>& X, unsigned int dim_idx) override;

        void fit(const Eigen::MatrixX<T>& X, const Eigen::VectorX<T>& y) override;

        void fit(const Eigen::MatrixX<T>& X, const Eigen::VectorX<T>& y, BoundaryConstraint boundary);

        Eigen::VectorX<T> operator()(const Eigen::MatrixX<T>& X) override;

};

#endif