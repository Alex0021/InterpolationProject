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
    for (unsigned int j=0; j<this->_X_data.rows(); j++)
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
void BarycentricInterpolator<T>::add_data(const Eigen::MatrixX<T>& X, const Eigen::VectorX<T>& y)
{
    if (X.cols() > 1)
    {
        throw BarycentricInterpolatorException::MultidimensionalImplementation("No multidimensional support yet for barycentric interpolation!", __func__);
    }
    // Extends existing data
    Eigen::MatrixX<T> new_X_data(this->_X_data.rows() + X.rows(), this->_X_data.cols());
    Eigen::VectorX<T> new_y_data(this->_y_data.rows() + y.rows());
    new_X_data << this->_X_data, X;
    new_y_data << this->_y_data, y;

    // 1D case
    // First update all previous weights
    Eigen::MatrixX<T> new_weights(this->_weights.rows() + X.rows(), this->_weights.cols());
    for (unsigned int i=0; i<this->_weights.rows();i++)
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
        new_weights(i,0) = this->_weights(i,0) / new_wi;
    }
    // Second compute new weights w_k
    for (unsigned int k= this->_X_data.rows()-1; k<new_X_data.rows(); k++)
    {
        T new_wi=1;
        for (unsigned int i=0; i<new_X_data.rows(); i++)
        {
            if (i != k)
            {
                new_wi *= (new_X_data(k,0) - new_X_data(i,0));
                if (new_wi == 0)
                {
                    throw BarycentricInterpolatorException::DivisionByZero("Cannot add an already existing interpolation datapoint!", __func__);
                }
            }
        }
        new_weights(k,0) = 1 / new_wi;
    }

    // Sets new data
    this->_X_data = new_X_data;
    this->_y_data = new_y_data;
    this->_weights = new_weights;

    // Readjust range
    this->_calculate_range();
}

template <typename T>
void BarycentricInterpolator<T>::add_data(T x, T y)
{
    // Add new values to X and y data
    Eigen::MatrixX<T> new_X_data(this->_X_data.rows() + 1, this->_X_data.cols());
    Eigen::VectorX<T> new_y_data(this->_y_data.rows() + 1);
    new_X_data << this->_X_data, x;
    new_y_data << this->_y_data, y;

    // First update all previous weights
    Eigen::MatrixX<T> new_weights(this->_weights.rows() + 1, this->_weights.cols());
    for (unsigned int i=0; i<this->_weights.rows();i++)
    {
        T new_wi = this->_X_data(i,0) - x;
        if (new_wi == 0)
        {
            throw BarycentricInterpolatorException::DivisionByZero("Cannot add an already existing interpolation datapoint!", __func__);
        }
        new_weights(i,0) = this->_weights(i,0) / new_wi;
    }

    // Second compute new weight w_k
    T new_wi=1;
    for (unsigned int i=0; i<this->_X_data.rows(); i++)
    {
        new_wi *= (x - this->_X_data(i,0));
        if (new_wi == 0)
        {
            throw BarycentricInterpolatorException::DivisionByZero("Cannot add an already existing interpolation datapoint!", __func__);
        }
    }
    new_weights(Eigen::placeholders::last, 0) = 1 / new_wi;

    // Sets new data
    this->_X_data = new_X_data;
    this->_y_data = new_y_data;
    this->_weights = new_weights;

    // Readjust range
    this->_calculate_range();
}

template <typename T>
void BarycentricInterpolator<T>::fit(const Eigen::MatrixX<T>& X, unsigned int dim) 
{
    // Store datapoints
    PolynomialInterpolator<T>::fit(X, dim);
    // Compute the weights
    if (this->_X_data.cols() > 1)
    {
        throw BarycentricInterpolatorException::MultidimensionalImplementation("No multidimensional support yet for barycentric interpolation!", __func__);
    }
    else
    {   
        this->_weights = Eigen::MatrixX<T>(this->_X_data.rows(),1);
        for (unsigned int i=0; i<this->_X_data.rows(); i++)
        {
            this->_weights(i,0) = this->_barycentric_weight(i);
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
        this->_weights = Eigen::MatrixX<T>(X.rows(),1);
        for (unsigned int i=0; i<X.rows(); i++)
        {
            _weights(i,0) = this->_barycentric_weight(i);
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
    // Check if x within the interpolation range
    if (x > this->_X_max(0) || x < this->_X_min(0))
    {
        throw BarycentricInterpolatorException::Extrapolation(x, this->_X_min(0), this->_X_max(0), __func__);
    }

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
        intermediate = this->_weights(j,0) / (x - this->_X_data(j,0));
        sum += intermediate;
        weighted_sum += intermediate * this->_y_data(j);
    }
    return weighted_sum / sum;
}

template class BarycentricInterpolator<int>;
template class BarycentricInterpolator<float>;
template class BarycentricInterpolator<double>;