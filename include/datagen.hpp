#pragma once

#include <Eigen/Core>
#include <fstream>
#include <filesystem>
#include <vector>
#include <utility>
#include <iostream>

// #ifndef DATAGEN_HPP 
// #define DATAGEN_HPP

template <typename T>
struct datagen {
    /**
     * @brief 
     * 
     * @tparam T 
     * @param path write the data to this file
     * @param n number of points
     * @param m number of features
     * @param degree degree of the polynomial
     */

    enum class PointGeneration {
        RANDOM_UNIFORM,
        CHEBYSHEV,
        UNIFORM,
    };

    static void write(const std::filesystem::path path, const Eigen::MatrixX<T> &data);

    static void random_data(std::filesystem::path path, int n, int m);

    static void polynomial(Eigen::MatrixX<T> &data, Eigen::VectorX<T> &coeffs, std::vector<std::pair<T, T>> range, PointGeneration pg);
    
    static void polynomial(Eigen::MatrixX2<T> &data, Eigen::VectorX<T> &coeffs, std::pair<T, T> range, PointGeneration pg);
    
    static void polynomial(Eigen::VectorX<T> &data, Eigen::VectorX<T> &x, Eigen::VectorX<T> &coeffs);
    
    static void chebyshev_polynomial(Eigen::VectorX<T> &data, int d, std::pair<T, T> range, PointGeneration pg);

    static void generate_points(Eigen::VectorX<T> &x, std::pair<T, T> range, PointGeneration pg);

    static void chebyshev_points(Eigen::VectorX<T> &x, std::pair<T, T> range);

    static void damped_cosine(Eigen::MatrixX2<T> &data, Eigen::VectorX<T> &params, std::pair<T, T> range, PointGeneration pg);

};

// #endif