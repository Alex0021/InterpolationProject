#include "datagen.hpp"
#include <iostream>

template <typename T>
void datagen::random_data(std::filesystem::path path, int n, int m) {
    std::ofstream file(path);
    std::cout << "Writing random data to " << path << std::endl;
    Eigen::MatrixX<T> data = Eigen::MatrixX<T>::Random(n, m);
    file << n << " " << m << std::endl;
    file << data;
    file.close();
}

template void datagen::random_data<double>(std::filesystem::path path, int n, int m);
template void datagen::random_data<float>(std::filesystem::path path, int n, int m);
template void datagen::random_data<int>(std::filesystem::path path, int n, int m);