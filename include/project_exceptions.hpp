#include <exception>
#include <string>
#include <format>
#include <tuple>

#ifndef __PROJECT_EXCEPTIONS_INCLUDE
#define __PROJECT_EXCEPTIONS_INCLUDE

/**
 * @brief Custom exception class to be used within the project
 * 
 */
class CustomException: public std::exception
{
    protected:
        std::string _msg;
        std::string _where;

    public:
        /**
         * @brief Construct a new Custom Exception object
         * 
         * @param msg Exception message
         */
        CustomException(const std::string& msg)
            : _msg(msg) {}

        /**
         * @brief Construct a new Custom Exception object
         * 
         * @param msg Exception message
         * @param where  Function where the exception was thrown
         */
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
        /**
         * @brief Construct a new Interpolation Project Exception object
         * 
         * @param msg Exception message
         */
        InterpolationProjectException(const std::string& msg)
            : CustomException(msg) {}

        /**
         * @brief Construct a new Interpolation Project Exception object
         * 
         * @param msg Exception message
         * @param where Function where the exception was thrown
         */
        InterpolationProjectException(const std::string& msg, const std::string& where)
            : CustomException(msg, where) {}

        /**
         * @brief Defines an index out of bounds exception when specifying the dimension
         * to use within the X matrix. Should never be < 0 or > #cols-1 of X
         */
        class IndexOutOfBounds: public CustomException {
            public: 
                /**
                 * @brief Construct a new Index Out Of Bounds object
                 * 
                 */
                IndexOutOfBounds()
                    : CustomException("The specified index for the interpolator is out of bounds") {}

                /**
                 * @brief Construct a new Index Out Of Bounds object
                 * 
                 * @param where Function where the exception was thrown
                 */
                IndexOutOfBounds(const std::string& where)
                    : CustomException("The specified index for the interpolator is out of bounds", where) {}

                /**
                 * @brief Construct a new Index Out Of Bounds object
                 * 
                 * @param request_idx Requested index
                 * @param max_idx Maximum index allowed
                 */
                IndexOutOfBounds(int request_idx, int max_idx)
                    : CustomException(std::format("The interpolation index {} is out of bounds! Should be in the range [0,{}]", request_idx, max_idx)) {}

                /**
                 * @brief Construct a new Index Out Of Bounds object
                 * 
                 * @param request_idx Requested index
                 * @param max_idx Maximum index allowed
                 * @param where Function where the exception was thrown
                 */
                IndexOutOfBounds(int request_idx, int max_idx, const std::string& where)
                    : CustomException(std::format("The interpolation index {} is out of bounds! Should be in the range [0,{}]", request_idx, max_idx), where) {}
        };

        /**
         * @brief Defines a size mismatch exception when trying to fit N datapoints to a M dimension y vector
         * where N != M
         */
        class SizeMismatch: public CustomException {
            public:
                /**
                 * @brief Construct a new Size Mismatch object
                 * 
                 */
                SizeMismatch(int actual, int expected)
                    : CustomException(std::format("The given interpolation labels of size {} do no match the number of datapoints {}!\n", actual, expected)) {}

                /**
                 * @brief Construct a new Size Mismatch object
                 * 
                 * @param actual Actual size of the interpolation labels
                 * @param expected Expected size of the interpolation labels
                 * @param where Function where the exception was thrown
                 */
                SizeMismatch(int actual, int expected, const std::string& where)
                    : CustomException(std::format("The given interpolation labels of size {} do no match the number of datapoints {}!\n", actual, expected), where) {}
        };      

        /**
         * @brief Defines an exception when trying to perform an operation on a non-square matrix
         * 
         */
        class MultidimensionalImplementation: public CustomException
        {
            public:
                /**
                 * @brief Construct a new Multidimensional Implementation object
                 * 
                 * @param msg Exception message
                 * @param where Function where the exception was thrown
                 */
                MultidimensionalImplementation(const std::string& msg, const std::string& where)
                    : CustomException(msg, where) {}

                /**
                 * @brief Construct a new Multidimensional Implementation object
                 * 
                 * @param msg Exception message
                 */
                MultidimensionalImplementation(const std::string& msg)
                    : CustomException(msg) {}
        };

        /**
         * @brief Defines an exception when trying to perform an extrapolation
         * 
         */
        class Extrapolation: public CustomException
        {
            public: 
                /**
                 * @brief Construct a new Extrapolation object
                 * 
                 * @param msg Exception message
                 * @param where Function where the exception was thrown
                 */
                Extrapolation(const std::string& msg, const std::string& where)
                    : CustomException(msg, where) {}

                /**
                 * @brief Construct a new Extrapolation object
                 * 
                 * @param msg Exception message
                 */
                Extrapolation(const std::string& msg)
                    : CustomException(msg) {} 

                /**
                 * @brief Construct a new Extrapolation object
                 * 
                 * @param value_error Value that was out of range
                 * @param min Minimum value allowed
                 * @param max Maximum value allowed
                 */
                Extrapolation(double value_error, double min, double max, const std::string& where)
                    : CustomException(std::format("Cannot perform extrapolation! Value {} is out of range [{} , {}]",
                                            value_error, min, max), where) {}
        };

        /**
         * @brief Defines an exception when trying to perform an operation on a non-square matrix
         * 
         */
        class InvalidType: public CustomException
        {
            public:
                /**
                 * @brief Construct a new Invalid Type object
                 * 
                 * @param msg Exception message
                 * @param where Function where the exception was thrown
                 */
                InvalidType(const std::string& msg, const std::string& where)
                    : CustomException(msg, where) {}

                /**
                 * @brief Construct a new Invalid Type object
                 * 
                 * @param msg Exception message
                 */
                InvalidType(const std::string& msg)
                    : CustomException(msg) {}
        };

        /**
         * @brief Defines an exception when trying to perform an operation on a non-square matrix
         * 
         */
        class DivisionByZero: public CustomException {

            public:
                /**
                 * @brief Construct a new Division By Zero object
                 * 
                 * @param msg Exception message
                 * @param where Function where the exception was thrown
                 */
                DivisionByZero(const std::string& msg)
                    : CustomException(msg) {}

                /**
                 * @brief Construct a new Division By Zero object
                 * 
                 * @param msg Exception message
                 * @param where Function where the exception was thrown
                 */
                DivisionByZero(const std::string& msg, const std::string& where)
                    : CustomException(msg, where) {}

                /**
                 * @brief Construct a new Division By Zero object
                 * 
                 */
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
{};

/**
 * @brief Contains specific exceptions used within the BarycentricInterpolator if any
 * 
 */
class BarycentricInterpolatorException: public InterpolationProjectException
{};

/**
 * @brief Contains specific exceptions used within the DataReader if any
 * 
 */
class DataReaderException: public InterpolationProjectException
{
    public:
        /**
         * @brief Construct a new Data Reader Exception object
         * 
         * @param msg Exception message
         * @param where Function where the exception was thrown
         */
        DataReaderException(const std::string& msg, const std::string& where)
            : InterpolationProjectException(msg, where)
        {};

        /**
         * @brief Construct a new Data Reader Exception object
         * 
         * @param msg Exception message
         */
        DataReaderException(const std::string& msg)
            : InterpolationProjectException(msg, "DataReader")
        {};

        /**
         * @brief Defines an exception when trying to read a file that does not exist
         * 
         */
        class FileNotFound: public CustomException
        {
            public:
                /**
                 * @brief Construct a new File Not Found object
                 * 
                 * @param filepath Filepath that was not found
                 */
                FileNotFound(const std::string& filepath) 
                    : CustomException(std::format("Could not find the specified filepath! --> '{}'", filepath), "DataReader")
                {};
        };

        /**
         * @brief Defines an exception when trying to read a file that does not exist
         * 
         */
        class NotANumber: public CustomException
        {
            public:
                /**
                 * @brief Construct a new Not A Number object
                 * 
                 * @param value_error Value that was not a number
                 */
                NotANumber(const std::string& value_error)
                    : CustomException(std::format("Numerical data expected for datapoints file! Found --> '{}'", value_error), "DataReader")
                {};
        };
};

/**
 * @brief Contains specific exceptions used within the CubicSplineInterpolator if any
 * 
 */
class CubicSplineInterpolatorException: public InterpolationProjectException{};

#endif