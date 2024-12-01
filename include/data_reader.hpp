#include <Eigen/Core>
#include <filesystem>

namespace DataReader {
    template <typename T>
    Eigen::MatrixX<T> read(std::filesystem::path path);
};