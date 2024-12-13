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
    /**
     * @brief The gnuplot object used to plot the data
     * 
     */
    Gnuplot gp;

    /**
     * @brief The number of plots that have been added to the plotter
     * 
     */
    void _plot_from_file(std::filesystem::path path);

public:

    /**
     * @brief Construct a new Plotter object
     * 
     */
    Plotter();
    ~Plotter();
    
    /**
     * @brief Plot data from a file
     * 
     * @param path : path to the file containing the data to plot
     */
    void plot(std::filesystem::path path);

    /**
     * @brief Plot data from a matrix
     * 
     * @param data : The data to plot
     */
    void _plot(const Eigen::MatrixX2<T> &data);
    
    /**
     * @brief Plot data from a matrix
     * 
     * @param data : The data to plot
     */
    void plot(const Eigen::MatrixX2<T> &data);
    
    /**
     * @brief Plot data from a matrix
     * 
     * @param data : The data to plot
     * @param title : The title of the plot
     */
    void plot_points(const Eigen::MatrixX2<T> &data, std::string title);

    /**
     * @brief Plot data from a matrix
     * 
     * @param data : The data to plot
     * @param title : The title of the plot
     */
    void plot_lines(const Eigen::MatrixX2<T> &data, std::string title);

    /**
     * @brief  Plot data from two matrices
     * 
     * @param n The number of matrices to plot
     * @param paths The paths to the files containing the data to plot
     * @param titles The titles of the plots
     * @param styles The styles of the plots 
     */
    void plot(int n, std::filesystem::path* paths, std::string* titles, std::string* styles);

    /**
     * @brief  Plot data from two matrices
     * 
     * @param point_data The data to plot as points
     * @param line_data The data to plot as lines
     */
    void plot(const Eigen::MatrixX2<T> &point_data, const Eigen::MatrixX2<T> &line_data);

    /**
     * @brief  Plot data from two matrices
     * 
     * @param point_data The data to plot as points
     * @param line_data The data to plot as lines
     * @param title The title of the plot
     */
    void plot(const Eigen::MatrixX2<T> &point_data, const Eigen::MatrixX2<T> &line_data, std::string title);

    /**
     * @brief  Plot data from two matrices
     * 
     */
    void reset();

    /**
     * @brief  Plot data from two matrices
     * 
     * @param cmd The command to send to gnuplot
     */
    void operator<<(const std::string& cmd);

};