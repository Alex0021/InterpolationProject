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

/**
 * @brief 
 * 
 * @tparam T : type of the data to plot. When T is int, plot on a lattice. 
 */
template <typename T>
class Plotter {

private:

    struct PlotterParams {
        std::string title = NULL;
        std::string xlabel = "x";
        std::string ylabel = "y";
        std::string zlabel = "z";
    };;

    /**
     * @brief plot the data from a given text file
     * @param filename 
     */
    void _plot_from_file(std::string filename);

public:

    Plotter();
    ~Plotter();
    
    /**
     * @brief 
     * 
     * @param filename plot the data from a given text file
     */
    void plot(std::string filename);

    /**
     * @brief 
     * 
     * @param data plot the data from a matrix. The dependent variable is the last column of the matrix.
     */
    void plot(Eigen::MatrixX<T> data);

    /**
     * @brief 
     * 
     * @param data 
     * @param dim : Dimension of the dependent variable of the data. If dim=1 and data has 2 columns (x,y), then we plot y=f(x). Whereas if dim=0, we plot x=f(y).
     */
    void plot(Eigen::MatrixX<T> data, int dim);

    void plot(std::VectorX<T> x, std::VectorX<T> y);

    void plot(std::VectorX<T> x, std::VectorX<T> y, std::VectorX<T> z);

    void reset();

    void show();

    void save(std::string filename);

    // Setters =========================================================================================================

    void set_title(std::string title);

    void set_xlabel(std::string xlabel);

    void set_ylabel(std::string ylabel);

    void set_zlabel(std::string zlabel);

    void set_labels(std::string xlabel, std::string ylabel, std::string zlabel);

    void set_labels(std::string xlabel, std::string ylabel);

};