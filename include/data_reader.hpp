#pragma once

#include <Eigen/Core>
#include <filesystem>
#include <fstream>

template <typename T>
struct DataReader {
    
    static Eigen::MatrixX<T> read(std::filesystem::path path);
    
    static Eigen::MatrixX<T> read(std::ifstream &file);

};