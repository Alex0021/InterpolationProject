#include "plotter.hpp"

void Plotter::plot(std::string filename) {
    _plot_from_file(filename);
}

void Plotter::_plot_from_file(std::string filename) {
    DataReader reader;
    Eigen::MatrixXf data = reader.read(filename);
    plot(data);
}


