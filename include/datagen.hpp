#include <Eigen/Core>
#include <fstream>

namespace datagen {
    /**
     * @brief 
     * 
     * @tparam T 
     * @param path write the data to this file
     * @param n number of points
     * @param m number of features
     * @param degree degree of the polynomial
     */
    template <typename T>
    void random_polynomial(std::filesystem::path path, int n, int m, int degree);

    template <typename T>
    void random_data(std::filesystem::path path, int n, int m);
}
