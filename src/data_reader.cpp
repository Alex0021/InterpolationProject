#include "data_reader.hpp"

template <typename T>
Eigen::MatrixX<T> DataReader<T>::read(std::filesystem::path path) {
    std::ifstream file(path);
    return DataReader<T>::read(file);
}

template <typename T>
Eigen::MatrixX<T> DataReader<T>::read(std::ifstream &file) {
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
    file.close();
    return data;
}

template class DataReader<double>;
template class DataReader<float>;
template class DataReader<int>; 