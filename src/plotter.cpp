#include "plotter.hpp"

template <typename T>
Plotter<T>::Plotter() {}

template <typename T>
Plotter<T>::~Plotter() {
    this->gp << "exit\n";
}

template <typename T>
void Plotter<T>::plot(std::filesystem::path path) {
    _plot_from_file(path);
}

template <typename T>
void Plotter<T>::plot(Eigen::MatrixX2<T> &data) {
    this->gp << "plot '-' with lines\n";
    _plot(data);
}

template <typename T>
void Plotter<T>::plot(Eigen::MatrixX2<T> &point_data, Eigen::MatrixX2<T> &line_data) {
    this->gp << "plot '-' with points, '-' with lines\n";
    _plot(point_data);
    _plot(line_data);
}

template <typename T>
void Plotter<T>::_plot_from_file(std::filesystem::path path) {
    Eigen::MatrixX2<T> data = DataReader::read<T>(path);
    this->plot(data);
}

template <typename T>
void Plotter<T>::_plot(Eigen::MatrixX2<T> &data) {
    Eigen::VectorX<T> x = data.col(0);
    Eigen::VectorX<T> y = data.col(1);
    this->gp.send1d(std::make_tuple(x, y));
}

template class Plotter<double>;
template class Plotter<float>;
template class Plotter<int>;


