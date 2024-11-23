#include <Eigen/Core>
#include <utility>
#include <filesystem>

class DataReader {

public:
    static std::pair<Eigen::Matrix<double, -1, -1>, Eigen::Matrix<double, -1, 1>> read(std::filesystem::path& filepath);

};