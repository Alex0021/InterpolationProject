#ifndef __BARYCENTRIC_INTERPOLATOR_INCLUDE
#define __BARYCENTRIC_INTERPOLATOR_INCLUDE

#include "Eigen/Core"
#include "polynomial_interpolator.hpp"

/**
 * @brief Class implementing the barycentric interpolation scheme
 *
 * @tparam T datapoints type (int, float, double)
 */
template <typename T>
class BarycentricInterpolator: public PolynomialInterpolator<T>
{
    private:
        Eigen::MatrixX<T> _weights;

        /**
         * @brief Compute the weight i,j for the multidemnsional case (m>1)
         * @param i row index for the weight matrix
         * @param j column (dimension) index for the weight matrix (if m>1)
         * @return calculated weight for x_ij
         */
        T _barycentric_weight(unsigned int i, unsigned int j);

        /** @brief Compute the weights defined as follow (2D):
         * \f[
         *      w_i = \left[ \prod_{j,j \neq i}^n (x_i-x_j) \right]^{-1}
         * \f]
         * @param i row index for the weight matrix
         * @return calculated weight for x_ij
         */
        T _barycentric_weight(unsigned int i);

    public:

        /**
         * @brief Constructor for the BarycentricInterpolator object
         */
        BarycentricInterpolator();

        /**
         * @brief  Adds new data (N datapoints) to the interpolator.
         * 
         *  Recomputes only the necessary weights
         * 
         * @param X new datapoints to be added
         * @param y corresponding interpolation values
         */
        void add_data(const Eigen::MatrixX<T>& X, const Eigen::VectorX<T>& y);

        /**
         * @brief Adds a single new datapoint (1D) to the interpolation
         * 
         * @param x datapoint to add
         * @param y function value @x
         */
        void add_data(T x, T y);

        void fit(const Eigen::MatrixX<T>& X, unsigned int dim) override;

        void fit(const Eigen::MatrixX<T>& X, const Eigen::VectorX<T>& y) override;

        Eigen::VectorX<T> operator()(const Eigen::MatrixX<T>& X) override;

        T operator()(T X) override;
};

#endif