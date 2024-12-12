/**
 * @file Plotter.hpp
 * @author Max Brodeur (maxbrod21@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2024-11-28
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#include <gnuplot-iostream.h>
#include <string>
#include "data_reader.hpp"
#include <Eigen/Core>
#include <vector>
#include <tuple>
#include <filesystem>
#include <format>

/**
 * @brief A class to plot data using gnuplot.
 * 
 * @tparam T : type of the data to plot. When T is int, plot on a lattice. 
 */
template <typename T>
class Plotter {

private:

    Gnuplot gp;

    void _plot_from_file(std::filesystem::path path);

public:

    Plotter();
    ~Plotter();
    
    void plot(std::filesystem::path path);

    void _plot(const Eigen::MatrixX2<T> &data);

    void plot(const Eigen::MatrixX<T> &data);
    
    void plot(const Eigen::MatrixX2<T> &data);
    
    void plot(const Eigen::MatrixX2<T> &data, std::string title);

    void plot_points(const Eigen::MatrixX2<T> &data, std::string title);

    void plot_lines(const Eigen::MatrixX2<T> &data, std::string title);

    void plot(int n, std::filesystem::path* paths, std::string* titles, std::string* styles);

    void plot(const Eigen::MatrixX2<T> &point_data, const Eigen::MatrixX2<T> &line_data);

    void plot(const Eigen::MatrixX2<T> &point_data, const Eigen::MatrixX2<T> &line_data, std::string title);

    void plot(const Eigen::MatrixX<T> &data, int dim);

    void plot(const Eigen::VectorX<T> &x, const Eigen::VectorX<T> &y);

    void plot(const Eigen::VectorX<T> &x, const Eigen::VectorX<T> &y, const Eigen::VectorX<T> &z);

    void reset();

    void show();

    void save(std::filesystem::path path);

    void operator<<(const std::string& cmd);


};