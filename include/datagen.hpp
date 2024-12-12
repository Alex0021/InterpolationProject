/**
 * @file datagen.hpp
 * @author Max Brodeur (maxbrod21@gmail.com)
 * @brief A utility class for generating and writing data to test interpolation algorithms. 
 * Contains methods for generating data of different sources, such as random, polynomial, and damped cosine functions. 
 * 
 * @version 0.1
 * @date 2024-12-12
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#pragma once

#include <Eigen/Core>
#include <fstream>
#include <filesystem>
#include <vector>
#include <utility>
#include <iostream>

// #ifndef DATAGEN_HPP 
// #define DATAGEN_HPP

/**
 * @brief A utility class for generating and writing data to test interpolation algorithms.
 * 
 * @tparam T 
 */
template <typename T>
struct datagen {

    /**
     * @brief Point generation paradigms to populate the domain of the data.
     * 
     */
    enum class PointGeneration {
        RANDOM_UNIFORM,
        CHEBYSHEV,
        UNIFORM,
    };

    /**
     * @brief Write the data to a file at the specified path. It is space separated and newline delimited. The first rows start with a '#' and contain metadata, such as the dimensions of the data.
     * TODO: be more specific about the format of the data, once we have chosen the convention. 
     * 
     * @param path : The path to write the data to
     * @param data : The data to write
     */
    static void write(const std::filesystem::path path, const Eigen::MatrixX<T> &data);

    /**
     * @brief Generate random data of size n \times m and write it to the specified path.
     * 
     * @param path : The path to write the data to
     * @param n : The number of rows
     * @param m : The number of columns
     */
    static void random_data(std::filesystem::path path, int n, int m);

    /**
     * @brief The multi-dimensional polynomial counterpart to the polynomial method. **Unsuppported**
     * 
     * @param data 
     * @param coeffs 
     * @param range 
     * @param pg 
     */
    static void polynomial(Eigen::MatrixX<T> &data, Eigen::VectorX<T> &coeffs, std::vector<std::pair<T, T>> range, PointGeneration pg);

    /**
     * @brief Generate a polynomial function of the specified coefficients and write it to the specified path. 
     * The degree of the polynomial is determined by the size of the coefficients vector.
     * E.g. for a polynomial of degree n, the coefficients vector should have n+1 elements.:
     * \f[
     * f(x) = a_0 + a_1 x + a_2 x^2 + \ldots + a_n x^n
     * \f]
     * 
     * @param data : The data to write the polynomial to. So both the domain (data.col(0)) and the range (data.col(1)) of the polynomial.
     * @param coeffs : The coefficients of the polynomial
     * @param range : The range of the data to generate
     * @param pg : The point generation paradigm to use
     */    
    static void polynomial(Eigen::MatrixX2<T> &data, Eigen::VectorX<T> &coeffs, std::pair<T, T> range, PointGeneration pg);
    
    /**
     * @brief Helper function to generate a polynomial function of the specified coefficients and populate the data vector with the values.
     * 
     * @param data : The vector to populate with the polynomial values
     * @param x : The domain of the polynomial
     * @param coeffs : The coefficients of the polynomial
     */
    static void polynomial(Eigen::VectorX<T> &data, Eigen::VectorX<T> &x, Eigen::VectorX<T> &coeffs);

    /**
     * @brief Generate a set of points according to the specified paradigm. 
     * 
     * @param x : The vector to populate with the points
     * @param range : The range of the data to generate
     * @param pg : The point generation paradigm to use
     */
    static void generate_points(Eigen::VectorX<T> &x, std::pair<T, T> range, PointGeneration pg);

    /**
     * @brief Implementation of the Chebyshev points generation paradigm.
     * The points are generated according to 
     * \f[
     * x_i = \frac{1}{2} \left( a + b \right) + \frac{1}{2} \left( b - a \right) \cos \left( \frac{2i + 1}{2n} \pi \right)
     * \f]
     * where a and b are the bounds of the range and n is the number of points to generate.
     * 
     * Note: The points are generated in the range (a, b) but the first and last points are not included in the range, therefore creating
     * a risk of extrapolation if the interpolation algorithm is not robust to it.
     * 
     * @param x : The vector to populate with the points
     * @param range : The range of the data to generate (a, b)
     */
    static void chebyshev_points(Eigen::VectorX<T> &x, std::pair<T, T> range);

    /**
     * @brief Generate a damped cosine function according to the specified parameters and write it to the specified path.
     * The damped cosine function is defined as:
     * \f[
     * f(x) = a e^{-bx^2} \cos(cx)
     * \f]
     * 
     * The parameters have the following effect:
     * - a : The amplitude of the cosine wave
     * - b : The damping factor (the rate at which the amplitude decreases)
     * - c : The frequency of the cosine wave
     * 
     * @param data : The data to write the damped cosine function to. So both the domain (data.col(0)) and the range (data.col(1)) of the function.
     * @param params : The parameters of the damped cosine function
     * @param range : The range of the data to generate
     * @param pg : The point generation paradigm to use
     */
    static void damped_cosine(Eigen::MatrixX2<T> &data, Eigen::VectorX<T> &params, std::pair<T, T> range, PointGeneration pg);

};

// #endif