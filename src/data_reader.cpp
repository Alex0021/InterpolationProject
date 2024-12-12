#include "data_reader.hpp"
#include "interpolators.hpp"
#include "project_exceptions.hpp"
#include <format>
#include <sstream>
#include <string>	
#include <vector>
#include <memory>

inline void check_path(const std::filesystem::path& path) 
{ std::filesystem::exists(path) ?  : throw DataReaderException::FileNotFound(path.c_str()); }

template <typename T>
Eigen::MatrixX<T> DataReader<T>::read(const std::filesystem::path& path) {
    // Check if file exists
    check_path(path);

    std::ifstream file(path);
    // Verify file is opened
    if (!file.is_open()) 
    {
        throw std::runtime_error(std::format("[reader] :: The provided file could not be opened! --> '{}'", path.c_str()));
    }
    return DataReader<T>::read(file);
}

template <typename T>
Eigen::MatrixX<T> DataReader<T>::read(std::ifstream &file) {
    int rows, cols;
    std::string s;
    // Sanity check for numbers
    const auto is_numeric = [](const std::string& s) {return !s.empty() && std::all_of(s.begin(), s.end(), [](const char c) { return std::isdigit(c); }); };
    
    file >> s;
    if (!is_numeric(s)) { throw DataReaderException::NotANumber("Number of rows in .txt file not a number!"); }
    rows = std::atoi(s.data());
    file >> s;
    if (!is_numeric(s)) { throw DataReaderException::NotANumber("Number of cols in .txt file not a number!"); }
    cols = std::atoi(s.data());

    Eigen::MatrixX<T> data(rows, cols);
    std::string s_val;
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            // Check if reached eof before filling up the data matrix
            if (file.eof()) {throw DataReaderException("End of data file reached before filling matrix --> wrong data size!"); }
            file >> s_val;
            // Using stringstream in order to keep original string value if conversion to T fails
            std::stringstream ss_val(s_val);
            T value;
            if(! (ss_val >> value)) { throw DataReaderException::NotANumber(s_val); }
            data(i, j) = value;
        }
    }
    file.close();
    return data;
}

template <typename T>
std::pair<std::unique_ptr<Interpolator<T>>, Eigen::MatrixX<T>> DataReader<T>::interpolator_from_file(const std::filesystem::path& path)
{
    // Check if file exists
    check_path(path);

    std::ifstream file(path);
    // Verify file is opened
    if (!file.is_open()) 
    {
        throw std::runtime_error(std::format("[reader] :: The provided file could not be opened! --> '{}'", path.c_str()));
    }

    // First line of the file should be interpolation scheme + params
    std::string interpolation_scheme;
    unsigned int fitting_dim;
    file >> interpolation_scheme;
    file >> fitting_dim;
    std::string line;
    // Retrieve remaining of the 1st line
    std::getline(file, line);
    std::stringstream ss_line(line);
    // Create array of options
    std::vector<std::string> options;
    std::string op;
    while(ss_line >> op) { options.push_back(op); }

    // Read data
    Eigen::MatrixX<T> X = DataReader<T>::read(file);

    // Construct interpolator object
    const auto to_uppercase = [](std::string& s) {return std::transform(s.begin(), s.end(), s.begin(),
                                                                                [](char c) {return std::toupper(c); }); };
    to_uppercase(interpolation_scheme);
    if (interpolation_scheme == "LAGRANGE")
    {
            std::unique_ptr<LagrangeInterpolator<T>> inter(new LagrangeInterpolator<T>);
            inter->fit(X, fitting_dim-1);
            return std::make_pair<std::unique_ptr<Interpolator<T>, Eigen::MatrixX<T>>(inter, X);
    }
    if (interpolation_scheme == "BARYCENTRIC")
    {
            std::unique_ptr<BarycentricInterpolator<T>> inter(new BarycentricInterpolator<T>);
            inter->fit(X, fitting_dim-1);
            return std::make_pair<std::unique_ptr<Interpolator<T>, Eigen::MatrixX<T>>(inter, X);
    }
    if (interpolation_scheme == "CUBIC_SPLINE")
    {
            // Check if any options provided
            std::unique_ptr<CubicSplineInterpolator<T>> inter;
            if (options.size() > 0)
            {
                inter = std::make_unique<CubicSplineInterpolator<T>>();
            }
            else 
            {
                inter = std::make_unique<CubicSplineInterpolator<T>>();
            }
            inter->fit(DataReader<T>::read(file), fitting_dim-1);
            return std::make_pair<std::unique_ptr<Interpolator<T>, Eigen::MatrixX<T>>(inter, X);
    }
    else
    {
        throw std::runtime_error("Unknown interpolator type!");
    }
}

template class DataReader<double>;
template class DataReader<float>;
template class DataReader<int>; 