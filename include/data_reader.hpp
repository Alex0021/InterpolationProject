#include <Eigen/Core>
#include <filesystem>
#include <fstream>

namespace DataReader {
    template <typename T>
    Eigen::MatrixX<T> read(std::filesystem::path path);
};