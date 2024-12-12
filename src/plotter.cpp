#include "plotter.hpp"

template <typename T>
Plotter<T>::Plotter() {
    this->gp << "plot \n";
}

template <typename T>
Plotter<T>::~Plotter() {
    this->gp << "exit\n";
}

template <typename T>
void Plotter<T>::plot(std::filesystem::path path) {
    _plot_from_file(path);
}

template <typename T>
void Plotter<T>::plot(const Eigen::MatrixX2<T> &data) {
    this->gp << "replot '-' with lines\n";
    _plot(data);
}

template <typename T>
void Plotter<T>::plot_lines(const Eigen::MatrixX2<T> &data, std::string title) {
    this->gp << std::format(" '-' with lines title '{}'\n", title);
    _plot(data);
}

template <typename T>
void Plotter<T>::plot_points(const Eigen::MatrixX2<T> &data, std::string title) {
    this->gp << std::format(" '-' with points title '{}'\n", title);
    _plot(data);
}

template <typename T>
void Plotter<T>::plot(const Eigen::MatrixX2<T> &point_data, const Eigen::MatrixX2<T> &line_data) {
    this->gp << "'-' with points title 'Data', '-' with lines title 'Interpolated'\n";
    _plot(point_data);
    _plot(line_data);
}

template <typename T>
void Plotter<T>::plot(int n, std::filesystem::path* paths, std::string* titles, std::string* styles) {
    this->gp << "plot ";
    for (int i = 0; i < n; i++) {
        this->gp << std::format("'{}' with {} title '{}', ", (std::string) paths[i], styles[i], titles[i]);
    }
    this->gp << "\n";
}

template <typename T>
void Plotter<T>::plot(const Eigen::MatrixX2<T> &point_data, const Eigen::MatrixX2<T> &line_data, std::string title) {
    this->gp << std::format("'-' with points title 'Data', '-' with lines title '{}'\n", title);
    _plot(point_data);
    _plot(line_data);
}

template <typename T>
void Plotter<T>::_plot_from_file(std::filesystem::path path) {
    Eigen::MatrixX2<T> data = DataReader<T>::read(path);
    this->plot(data);
}

template <typename T>
void Plotter<T>::_plot(const Eigen::MatrixX2<T> &data) {
    Eigen::VectorX<T> x = data.col(0);
    Eigen::VectorX<T> y = data.col(1);
    this->gp.send1d(std::make_tuple(x, y));
}

template <typename T>
void Plotter<T>::operator<<(const std::string& cmd) {
    this->gp << cmd;
}

template <typename T>
void Plotter<T>::reset() {
    this->gp << "plot ";
}

template class Plotter<double>;
template class Plotter<float>;
template class Plotter<int>;


