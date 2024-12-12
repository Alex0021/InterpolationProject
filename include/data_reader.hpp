#pragma once

#include <Eigen/Core>
#include <filesystem>
#include <fstream>
#include "interpolator.hpp"

/**
 * @brief Generic reader class to read datapoints from fromatted .txt files 
 * 
 * Format is the following (all space separated):
 * 
 * - Line 1: interpolation method name + params if needed
 * 
 * - Line 2: datasize (N M)
 * 
 * - Line 3: datapoints as columns (NxM)
 * 
 * @tparam T datapoints type (int, double, float)
 */
template <typename T>
struct DataReader { 

    /**
     * @brief Reads a file containing the data to be interpolated (expects right format)
     * 
     * @param path filepath
     * @return Eigen::MatrixX<T> Datapoints
     */
    static Eigen::MatrixX<T> read(const std::filesystem::path& path);
        
    /**
     * @brief Reads a file containing the data to be interpolated usispling an already initialized filestream
     * 
     * @param file input filestream to be read from
     * @return Eigen::MatrixX<T> Datapoints
     */
    static Eigen::MatrixX<T> read(std::ifstream &file);

    /**
     * @brief Reads a file containing the data and the interpolation method to directly generate
     * appropriate interpolator object
     * 
     * @param path filepath
     * @return Interpolator<T> the requested interpolator object 
     */
    static std::pair<std::unique_ptr<Interpolator<T>>, Eigen::MatrixX<T>> interpolator_from_file(const std::filesystem::path& path);

};