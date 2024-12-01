#include "data_reader.hpp"

template <typename T>
std::pair<Eigen::MatrixX<T>, Eigen::VectorX<T> > DataReader::read(std::filesystem::path& filepath) {
    Eigen::MatrixX<T> X;
    Eigen::VectorX<T> y;
    return std::make_pair(X, y);
}

// Explicit instantiation of templating 
template std::pair<Eigen::MatrixX<double>, Eigen::VectorX<double> > DataReader::read<double>(std::filesystem::path& filepath);
template std::pair<Eigen::MatrixX<float>, Eigen::VectorX<float> > DataReader::read<float>(std::filesystem::path& filepath);
template std::pair<Eigen::MatrixX<int>, Eigen::VectorX<int> > DataReader::read<int>(std::filesystem::path& filepath);

