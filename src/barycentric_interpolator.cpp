#include "barycentric_interpolator.hpp"
#include "project_exceptions.hpp"

template <typename T>
BarycentricInterpolator<T>::BarycentricInterpolator() {}

template <typename T>
T BarycentricInterpolator<T>::_barycentric_weight(unsigned int i, unsigned int j) 
{
    throw std::runtime_error("FUNCTION NOT YET IMPLEMENTED ==> barycentric_weight");
}

template <typename T>
T BarycentricInterpolator<T>::_barycentric_weight(unsigned int i) 
{
    // Check if X data is multidimensional
    if (this->_X_data.cols() > 1)
    {
        throw BarycentricInterpolatorException::MultidimensionalImplementation("Call proper weight calculation function if multidimensional!", __func__);
    }
    
    // Loop through all xj (j!=i)
    T xi = this->_X_data(i,0);
    T  wi = 1;
    for (unsigned int j=0; j<i; j++)
    {
        if (j == i) { continue; }
        T xj = this->_X_data(j,0);
        if (xj == xi) 
        {
            throw BarycentricInterpolatorException::DivisionByZero("Two datapoints have the same value for weight calculation!", __func__);
        }
        wi *= (xi-xj);
    }
    return 1.0 / wi;
}

template <typename T>
void BarycentricInterpolator<T>::add_data(Eigen::MatrixX<T> X)
{
    if (X.cols() > 1)
    {
        throw BarycentricInterpolatorException::MultidimensionalImplementation("No multidimensional support yet for barycentric interpolation!", __func__);
    }
    // 1D case
    // First update all previous weights
    Eigen::MatrixX<T> new_weights(this->weights.rows() + X.rows(), this->weights.cols());
    for (unsigned int i=0; i<this->weights.rows();i++)
    {
        T new_wi=1;
        for (unsigned int k=0; k<X.rows(); k++)
        {
            new_wi *= (this->_X_data(i,0) - X(k,0));
            if (new_wi == 0)
            {
                throw BarycentricInterpolatorException::DivisionByZero("Cannot add an already existing interpolation datapoint!", __func__);
            }
        }
        new_weights(i,0) = this->weights(i,0) / new_wi;
    }

    // Second compute new weights w_k
    for (unsigned int k=0; k<X.rows(); k++)
    {
        T new_wi=1;
        for (unsigned int i=0; i<X.rows(); i++)
        {
            new_wi *= (this->_X_data(i,0) - X(k,0));
            if (new_wi == 0)
            {
                throw BarycentricInterpolatorException::DivisionByZero("Cannot add an already existing interpolation datapoint!", __func__);
            }
        }
    }
}

template <typename T>
void BarycentricInterpolator<T>::fit(const Eigen::MatrixX<T>& X, unsigned int dim) 
{
    // Store datapoints
    PolynomialInterpolator<T>::fit(X, dim);
    // Compute the weights
    if (X.cols() > 1)
    {
        throw BarycentricInterpolatorException::MultidimensionalImplementation("No multidimensional support yet for barycentric interpolation!", __func__);
    }
    else
    {   
        this->weights = Eigen::MatrixX<T>(X.rows(),1);
        for (unsigned int i=0; i<X.rows(); i++)
        {
            weights(i,0) = this->_barycentric_weight(i);
        }
    }
}

template <typename T>
void BarycentricInterpolator<T>::fit(const Eigen::MatrixX<T>& X, const Eigen::VectorX<T>& y) 
{
    // Store datapoints
    PolynomialInterpolator<T>::fit(X, y);
    // Compute the weights
    if (X.cols() > 1)
    {
        throw BarycentricInterpolatorException::MultidimensionalImplementation("No multidimensional support yet for barycentric interpolation!", __func__);
    }
    else
    {
        this->weights = Eigen::MatrixX<T>(X.rows(),1);
        for (unsigned int i=0; i<X.rows(); i++)
        {
            weights(i,0) = this->_barycentric_weight(i);
        }
    }
}

template <typename T>
Eigen::VectorX<T> BarycentricInterpolator<T>::operator()(const Eigen::MatrixX<T>& X) 
{
    // Check for corresponding dimensions (m)
    if (X.cols() != this->_X_data.cols())
    {
        throw BarycentricInterpolatorException::SizeMismatch(X.cols(), this->_X_data.cols());
    }
    if (X.cols() > 1)
    {
        throw BarycentricInterpolatorException::MultidimensionalImplementation("No multidimensional support yet for barycentric interpolation!", __func__);
    }
    // Loop through all values
    Eigen::VectorX<T> y(X.rows());
    for (unsigned int i=0; i<X.rows(); i++)
    {
        y(i) = this->operator()(X(i,0));
    } 
    return y;
}

template <typename T>
T BarycentricInterpolator<T>::operator()(T x) 
{
    T weighted_sum=0, sum=0;
    T intermediate = 0;
    for (unsigned int j=0; j<this->_X_data.rows(); j++) 
    {
        // Check if the querry point is an exact point
        if (x - this->_X_data(j,0) == 0)
        {
            weighted_sum = this->_y_data(j);
            sum = 1;
            break;
        }
        intermediate = this->weights(j,0) / (x - this->_X_data(j,0));
        sum += intermediate;
        weighted_sum += intermediate * this->_y_data(j);
    }
    return weighted_sum / sum;
}

template class BarycentricInterpolator<int>;
template class BarycentricInterpolator<float>;
template class BarycentricInterpolator<double>;