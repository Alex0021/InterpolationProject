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

/**
 * @brief  Class implementing the Lagrange interpolation scheme
 *
 * @tparam T dataypes (int, float, double)
 */
template <typename T>
class LagrangeInterpolator: public PolynomialInterpolator<T> {

    private:
        /**
         * @brief Calculate basis value for datapoint i considering given interpolation point x
         * \f[
         *      l_i(x) = \prod_{j,j \neq i}^{n} \frac{x-x_j}{x_i-x_j}
         * \f]
         * @param i Index of the datapoint to fit the lagrange polynomial 
         * @param x_interpolation The currently interpolated datapoint x
         * @return Value of the i-th polynomial evaluated @x
         */
        T lagrange_basis(unsigned int i, const Eigen::VectorX<T>& x_interpolation);

    public:
        /**
         * @brief Constructs a Lagrange interpolator object
         */
        LagrangeInterpolator();

        Eigen::VectorX<T> operator()(const Eigen::MatrixX<T>& X) override;
};