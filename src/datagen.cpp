#include "datagen.hpp"
#include <iostream>

template <typename T>
void datagen<T>::random_data(std::filesystem::path path, int n, int m) {
    std::ofstream file(path);
    std::cout << "Writing random data to " << path << std::endl;
    Eigen::MatrixX<T> data = Eigen::MatrixX<T>::Random(n, m);
    file << n << " " << m << std::endl;
    file << data;
    file.close();
}

template <typename T>
void datagen<T>::polynomial(Eigen::VectorX<T> &data, Eigen::VectorX<T> &x, Eigen::VectorX<T> &coeffs) {
    int n = data.rows();
    data = coeffs[0]*Eigen::VectorX<T>::Ones(n);
    int deg = coeffs.size();
    Eigen::VectorX<T> y = Eigen::VectorX<T>::Ones(n);
    for (int i = 1; i < deg; i++) {
        y = y.cwiseProduct(x);
        data = data + (coeffs[i]*y).matrix();
    }
}

template <typename T>
void datagen<T>::polynomial(Eigen::MatrixX<T> &data, Eigen::VectorX<T> &coeffs, std::vector<std::pair<T, T>> range, datagen<T>::PointGeneration pg) {
    throw std::invalid_argument("Higher dimensions not yet implemented.");
}

template <typename T>
void datagen<T>::polynomial(Eigen::MatrixX2<T> &data, Eigen::VectorX<T> &coeffs, std::pair<T, T> range, datagen<T>::PointGeneration pg) {
    int n = data.rows();
    Eigen::VectorX<T> x(n);
    datagen<T>::generate_points(x, range, pg);
    Eigen::VectorX<T> y(n);
    datagen<T>::polynomial(y, x, coeffs);
    data.col(0) = x;
    data.col(1) = y;
}

template <typename T>
void datagen<T>::generate_points(Eigen::VectorX<T> &x, std::pair<T, T> range, datagen<T>::PointGeneration pg) {
    int n = x.rows();
    switch (pg) {
        case datagen<T>::PointGeneration::RANDOM_UNIFORM:
            x = Eigen::VectorX<T>::Random(n, 1);
            break;
        case datagen<T>::PointGeneration::CHEBYSHEV:
            throw std::invalid_argument("Chebyshev not implemented");
            break;
        case datagen<T>::PointGeneration::UNIFORM:
            x = Eigen::VectorX<T>::LinSpaced(n, range.first, range.second);
            break;
        default:
            throw std::invalid_argument("Invalid PointGeneration");
    }
}

template struct datagen<double>;