/**
 * @file DataReader.hpp
 * @author Alexandre Hebert (alexandre.hebert@epfl.ch)
 * @brief  File containing the class definition of the DataReader object
 * @version 0.1
 * @date 2024-11-28
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#include <Eigen/Core>
#include <utility>
#include <filesystem>
/**
 * @brief DataReader object for reading input from a .txt file
 * 
 * It reads N points of M dimensions defined in a text file
 * 
 */
class DataReader {

public:
    /**
     * @brief Reads a .txt file containing the N datapoints of dimension M
     * 
     * @tparam T Output datatype (int, float, double)
     * @param filepath .txt file location
     * @return std::pair<Eigen::MatrixX<T>, Eigen::VectorX<T> > X matrix and y vector
     */
    template <typename T>
    static std::pair<Eigen::MatrixX<T>, Eigen::VectorX<T> > read(std::filesystem::path& filepath);

};