#include "data_reader.hpp"
#include <fstream>
#include <filesystem>

template <typename T>
Eigen::MatrixX<T> DataReader::read(std::filesystem::path path) {
    std::ifstream file(path);

    int rows, cols;
    file >> rows >> cols;

    Eigen::MatrixX<T> data(rows, cols);
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            T value;
            file >> value;
            data(i, j) = value;
        }
    }

    return data;
}

template Eigen::MatrixX<double> DataReader::read<double>(std::filesystem::path path);
template Eigen::MatrixX<float> DataReader::read<float>(std::filesystem::path path);
template Eigen::MatrixX<int> DataReader::read<int>(std::filesystem::path path);