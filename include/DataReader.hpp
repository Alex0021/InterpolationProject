#include <Eigen/Core>
#include <utility>
#include <filesystem>

class DataReader {

public:
    template <typename T>
    static std::pair<Eigen::MatrixX<T>, Eigen::VectorX<T> > read(std::filesystem::path& filepath);
};