#include <exception>
#include <string>
#include <format>
#include <tuple>

#ifndef __PROJECT_EXCEPTIONS_INCLUDE
#define __PROJECT_EXCEPTIONS_INCLUDE

class CustomException: public std::exception
{
    protected:
        std::string _msg;
        std::string _where;

    public:
        CustomException(const std::string& msg)
            : _msg(msg) {}

        CustomException(const std::string& msg, const std::string& where)
            : _where(where)
        {
            _msg = std::format("[{}] :: {}", _where, msg);
        }

        const char* what() const noexcept override 
        {
            return _msg.data();
        }
};


/**
 * @brief Contains all the necessary exceptions for our Interpolation Project
 * 
 */
class InterpolationProjectException: public CustomException {

    public:
        InterpolationProjectException(const std::string& msg)
            : CustomException(msg) {}

        InterpolationProjectException(const std::string& msg, const std::string& where)
            : CustomException(msg, where) {}

        /**
         * @brief Defines an index out of bounds exception when specifying the dimension
         * to use within the X matrix. Should never be < 0 or > #cols-1 of X
         */
        class IndexOutOfBounds: public CustomException {
            public: 
                IndexOutOfBounds()
                    : CustomException("The specified index for the interpolator is out of bounds") {}

                IndexOutOfBounds(const std::string& where)
                    : CustomException("The specified index for the interpolator is out of bounds", where) {}

                IndexOutOfBounds(int request_idx, int max_idx)
                    : CustomException(std::format("The interpolation index {} is out of bounds! Should be in the range [0,{}]", request_idx, max_idx)) {}

                IndexOutOfBounds(int request_idx, int max_idx, const std::string& where)
                    : CustomException(std::format("The interpolation index {} is out of bounds! Should be in the range [0,{}]", request_idx, max_idx), where) {}
        };

        /**
         * @brief Defines a size mismatch exception when trying to fit N datapoints to a M dimension y vector
         * where N != M
         */
        class SizeMismatch: public CustomException {
            public:
                SizeMismatch(int actual, int expected)
                    : CustomException(std::format("The given interpolation labels of size {} do no match the number of datapoints {}!\n", actual, expected)) {}

                SizeMismatch(int actual, int expected, const std::string& where)
                    : CustomException(std::format("The given interpolation labels of size {} do no match the number of datapoints {}!\n", actual, expected), where) {}
        };      

        class MultidimensionalImplementation: public CustomException
        {
            public:
                MultidimensionalImplementation(const std::string& msg, const std::string& where)
                    : CustomException(msg, where) {}

                MultidimensionalImplementation(const std::string& msg)
                    : CustomException(msg) {}
        };

        class Extrapolation: public CustomException
        {
            public: 
                Extrapolation(const std::string& msg, const std::string& where)
                    : CustomException(msg, where) {}

                Extrapolation(const std::string& msg)
                    : CustomException(msg) {} 

                Extrapolation(double value_error, double min, double max, const std::string& where)
                    : CustomException(std::format("Cannot perform extrapolation! Value {} is out of range [{} , {}]",
                                            value_error, min, max), where) {}
        };

        class InvalidType: public CustomException
        {
            public:
                InvalidType(const std::string& msg, const std::string& where)
                    : CustomException(msg, where) {}

                InvalidType(const std::string& msg)
                    : CustomException(msg) {}
        };

        class DivisionByZero: public CustomException {

            public:
                DivisionByZero(const std::string& msg)
                    : CustomException(msg) {}

                DivisionByZero(const std::string& msg, const std::string& where)
                    : CustomException(msg, where) {}

                DivisionByZero()
                    : CustomException("Division by zero!") {}
        };
};

/**
 * @brief Contains specific exceptions used within the Polynomial Interpolators if any
 * 
 */
class PolynomialInterpolatorException: public InterpolationProjectException
{};

/**
 * @brief Contains specific exceptions used within the LagrangeInterpolator if any
 * 
 */
class LagrangeInterpolatorException: public InterpolationProjectException
{
};

class BarycentricInterpolatorException: public InterpolationProjectException
{
};

#endif