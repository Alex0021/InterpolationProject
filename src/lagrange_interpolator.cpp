#include "lagrange_interpolator.hpp"
#include "project_exceptions.hpp"

template <typename T>
T LagrangeInterpolator<T>::_lagrange_basis(unsigned int i, const Eigen::VectorX<T>& x_interpolation) 
{
    throw LagrangeInterpolatorException::MultidimensionalImplementation("M dimension interpolation is not yet implemented :/ Please try later!");
}

template <typename T>
T LagrangeInterpolator<T>::_lagrange_basis(unsigned int i, T x_interpolation) 
{
    Eigen::MatrixX<T> X = this->get_X_data();
    // Checking for only 1D datapoints in X
    if (X.cols() > 1)
    {
        throw LagrangeInterpolatorException::MultidimensionalImplementation("Impossible to use M-dimensional datapoints with 1D interpolation point. Call appropriate function or check fitted X data!");
    }
    // Checking for basis index within range of N datapoints
    if (i >= X.rows())
    {
        throw LagrangeInterpolatorException::IndexOutOfBounds(i, X.rows()-1);
    }
    // Calculate i-th basis
    T li = 1;
    for (unsigned int j=0; j<X.rows(); j++)
    {
        if (j != i)
        {
            // Checking for division by 0
            if (X(i,0) - X(j,0) == 0)
            {
                throw LagrangeInterpolatorException::DivisionByZero();
            }
            // Calculate the j-th element
            li *= (x_interpolation - X(j,0)) / (X(i,0) - X(j,0));
        }
    }
    return li;
};

template <typename T>
LagrangeInterpolator<T>::LagrangeInterpolator() {}

template <typename T>
Eigen::VectorX<T> LagrangeInterpolator<T>::operator()(const Eigen::MatrixX<T>& X) 
{
    // Check if multidimensional
    if (X.cols() > 1)
    {
        throw LagrangeInterpolatorException::MultidimensionalImplementation("M dimension interpolation is not yet implemented :/ Please try later!", __func__);
    }
    else
    {
        // Unidimensional case
        Eigen::VectorX<T> y_out(X.rows());
        for (unsigned int m=0; m<X.rows(); m++)
        {
            // Call the already implemented 1D 1 datapoint operator()
            y_out(m) = this->operator()(X(m,0));
        }
        return y_out;
    }
}

template <typename T>
T LagrangeInterpolator<T>::operator()(T x) 
{
    // Check if x within the interpolation range
    if (x > this->_X_max(0) || x < this->_X_min(0))
    {
        throw InterpolationProjectException::Extrapolation("Should not extrapolate!", __func__);
    }
    // Calculate y_m with basis l_i
    T y_m = 0;
    for (unsigned int i=0; i<this->get_X_data().rows(); i++)
    {
        y_m += this->_lagrange_basis(i, x)*this->get_y_data()(i);
    }
    return y_m;
}

template class LagrangeInterpolator<int>;
template class LagrangeInterpolator<float>;
template class LagrangeInterpolator<double>;
template class LagrangeInterpolator<long double>;
