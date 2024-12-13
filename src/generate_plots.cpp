#include <Eigen/Dense>
#include <filesystem>

#include <iostream>
#include "data_reader.hpp"
#include "datagen.hpp"
#include "plotter.hpp"
#include "lagrange_interpolator.hpp"
#include "fourier_approximator.hpp"
#include "cubic_spline_interpolator.hpp"

#include <unsupported/Eigen/FFT>
#include <vector>

void test_fft() {

    Eigen::VectorX<double> v1(10);
    v1 << -1, 1, -1, 1, -1, 1, -1, 1, -1, 1;

    FourierApproximator<double> fa;
    fa.fit(v1);
    
    int n = 100;
    Eigen::VectorX<double> v2 = fa.approximate_points(n);

    Eigen::VectorX<double> x1(v1.rows()); // original data
    Eigen::VectorX<double> x2(n); 
    datagen<double>::generate_points(x1, {-1, 1}, datagen<double>::PointGeneration::UNIFORM);
    datagen<double>::generate_points(x2, {-1, 1}, datagen<double>::PointGeneration::UNIFORM);
    Eigen::MatrixX2<double> data1(x1.rows(), 2);
    Eigen::MatrixX2<double> data2(x2.rows(), 2);
    data1 << x1, v1;
    data2 << x2, v2;

    Plotter<double> plotter;
    plotter.plot(data1, data2);

}

void test_damped_cosine() {
    Eigen::MatrixX2<double> data(50, 2);
    std::pair<double, double> range = {-10, 10};
    datagen<double>::PointGeneration pg = datagen<double>::PointGeneration::UNIFORM;
    Eigen::VectorX<double> params(3);
    params << 1, 0.1, 2;
    datagen<double>::damped_cosine(data, params, range, pg);

    LagrangeInterpolator<double> interpolator;
    interpolator.fit(data, 1);
    Eigen::VectorX<double> x(75);
    datagen<double>::generate_points(x, range, pg);
    Eigen::VectorX<double> interpolated = interpolator((Eigen::VectorX<double>)x);
    Eigen::MatrixX2<double> interpolated_data(x.rows(), 2);
    interpolated_data << x, interpolated;

    Plotter<double> plotter;
    plotter.plot(data, interpolated_data);
}

void test_damped_cosine_cubic() {

    int n = 20;
    int m = 2;

    Eigen::MatrixX2<double> data(n, m);
    std::pair<double, double> range = {-10, 10};
    datagen<double>::PointGeneration pg = datagen<double>::PointGeneration::UNIFORM;
    Eigen::VectorX<double> params(3);
    params << 1, 0.05, 5;
    
    datagen<double>::damped_cosine(data, params, range, pg);

    n = 1000;
    Eigen::VectorX<double> x(n);
    datagen<double>::generate_points(x, range, pg);

    CubicSplineInterpolator<double> cubic_interpolator;
    cubic_interpolator.fit(data, 1);
    Eigen::VectorX<double> cubic_interpolated = cubic_interpolator((Eigen::VectorX<double>)x);
    Eigen::MatrixX2<double> cubic_interpolated_data(n, 2);
    cubic_interpolated_data << x, cubic_interpolated;
    
    CubicSplineInterpolator<double> clamped_cubic_interpolator(CubicSplineInterpolator<double>::BoundaryConstraint::CLAMPED, {-2, 2});
    clamped_cubic_interpolator.fit(data, 1);
    Eigen::VectorX<double> clamped_cubic_interpolated = clamped_cubic_interpolator((Eigen::VectorX<double>)x);
    Eigen::MatrixX2<double> clamped_cubic_interpolated_data(n, 2);
    clamped_cubic_interpolated_data << x, clamped_cubic_interpolated;


    LagrangeInterpolator<double> lagrange_interpolator;
    lagrange_interpolator.fit(data, 1);
    Eigen::VectorX<double> lagrange_interpolated = lagrange_interpolator((Eigen::VectorX<double>)x);
    Eigen::MatrixX2<double> lagrange_interpolated_data(n, 2);
    lagrange_interpolated_data << x, lagrange_interpolated;


    std::string path = "data/damped_cosine_";
    std::filesystem::path paths[4] = {
        path+"data.txt",
        path+"cubic.txt", 
        path+"clamped_cubic.txt",
        path+"lagrange.txt", 
    };
    datagen<double>::write(paths[0], data);
    datagen<double>::write(paths[1], cubic_interpolated_data);
    datagen<double>::write(paths[2], clamped_cubic_interpolated_data);
    datagen<double>::write(paths[3], lagrange_interpolated_data);

    // plotter << "plot '-' with lines title 'Lagrange', '-' with lines title 'Cubic spline', '-' with points title 'Data'\n";
    std::string titles[4] = {
        "Data",
        "Cubic spline", 
        "Clamped Cubic spline", 
        "Lagrange", 
    };
    std::string styles[4] = {
        "points",
        "lines", 
        "lines", 
        "lines", 
    };
   
    Plotter<double> plotter;
    plotter.plot(3, paths, titles, styles);

}

void test_cosine_cubic() {

    int n = 10;
    int m = 2;

    Eigen::MatrixX2<double> data(n, m);
    std::pair<double, double> range = {-M_PI_2, M_PI_2};
    datagen<double>::PointGeneration pg = datagen<double>::PointGeneration::UNIFORM;
    Eigen::VectorX<double> params(3);
    params << 1, 0.05, 5;

    Eigen::VectorX<double> x(n);
    datagen<double>::generate_points(x, range, pg);
    data << x, (x.array().cos()).matrix();
    
    n = 50;
    x = Eigen::VectorX<double>(n);
    datagen<double>::generate_points(x, range, pg);

    CubicSplineInterpolator<double> cubic_interpolator;
    cubic_interpolator.fit(data, 1);
    Eigen::VectorX<double> cubic_interpolated = cubic_interpolator((Eigen::VectorX<double>)x);
    Eigen::MatrixX2<double> cubic_interpolated_data(n, 2);
    cubic_interpolated_data << x, cubic_interpolated;
    
    CubicSplineInterpolator<double> clamped_cubic_interpolator(CubicSplineInterpolator<double>::BoundaryConstraint::CLAMPED, {1, -1});
    clamped_cubic_interpolator.fit(data, 1);
    Eigen::VectorX<double> clamped_cubic_interpolated = clamped_cubic_interpolator((Eigen::VectorX<double>)x);
    Eigen::MatrixX2<double> clamped_cubic_interpolated_data(n, 2);
    clamped_cubic_interpolated_data << x, clamped_cubic_interpolated;


    LagrangeInterpolator<double> lagrange_interpolator;
    lagrange_interpolator.fit(data, 1);
    Eigen::VectorX<double> lagrange_interpolated = lagrange_interpolator((Eigen::VectorX<double>)x);
    Eigen::MatrixX2<double> lagrange_interpolated_data(n, 2);
    lagrange_interpolated_data << x, lagrange_interpolated;


    std::string path = "data/cosine_";
    std::filesystem::path paths[4] = {
        path+"data.txt",
        path+"cubic.txt", 
        path+"clamped_cubic.txt",
        path+"lagrange.txt", 
    };
    datagen<double>::write(paths[0], data);
    datagen<double>::write(paths[1], cubic_interpolated_data);
    datagen<double>::write(paths[2], clamped_cubic_interpolated_data);
    datagen<double>::write(paths[3], lagrange_interpolated_data);

    // plotter << "plot '-' with lines title 'Lagrange', '-' with lines title 'Cubic spline', '-' with points title 'Data'\n";
    std::string titles[4] = {
        "Data",
        "Cubic spline", 
        "Clamped Cubic spline", 
        "Lagrange", 
    };
    std::string styles[4] = {
        "points",
        "lines", 
        "lines", 
        "lines", 
    };
   
    Plotter<double> plotter;
    plotter.plot(3, paths, titles, styles);

}

void test_quadtratic_splines() {
    int n = 10;
    int m = 2;

    Eigen::MatrixX2<long double> data(n, m);
    std::pair<long double, long double> range = {-10, 10};
    datagen<long double>::PointGeneration pg = datagen<long double>::PointGeneration::UNIFORM;
    Eigen::VectorX<long double> params(3);
    // params << 1, 0.05, 5;
    params << 0, 0, 1;
    
    // datagen<long double>::damped_cosine(data, params, range, pg);
    datagen<long double>::polynomial(data, params, range, pg);

    n = 100;
    Eigen::VectorX<long double> x(n);
    datagen<long double>::generate_points(x, range, pg);

    CubicSplineInterpolator<long double> cubic_interpolator;
    cubic_interpolator.fit(data, 1);
    Eigen::VectorX<long double> cubic_interpolated = cubic_interpolator((Eigen::VectorX<long double>)x);
    Eigen::MatrixX2<long double> cubic_interpolated_data(n, 2);
    cubic_interpolated_data << x, cubic_interpolated;
    
    // CubicSplineInterpolator<long double> clamped_cubic_interpolator(CubicSplineInterpolator<long double>::BoundaryConstraint::CLAMPED, {-2, 2});
    // CubicSplineInterpolator<long double> clamped_cubic_interpolator(CubicSplineInterpolator<long double>::BoundaryConstraint::CLAMPED, {2, 4});
    CubicSplineInterpolator<long double> clamped_cubic_interpolator(CubicSplineInterpolator<long double>::BoundaryConstraint::CLAMPED, {-200, 200});
    clamped_cubic_interpolator.fit(data, 1);
    Eigen::VectorX<long double> clamped_cubic_interpolated = clamped_cubic_interpolator((Eigen::VectorX<long double>)x);
    Eigen::MatrixX2<long double> clamped_cubic_interpolated_data(n, 2);
    clamped_cubic_interpolated_data << x, clamped_cubic_interpolated;


    LagrangeInterpolator<long double> lagrange_interpolator;
    lagrange_interpolator.fit(data, 1);
    Eigen::VectorX<long double> lagrange_interpolated = lagrange_interpolator((Eigen::VectorX<long double>)x);
    Eigen::MatrixX2<long double> lagrange_interpolated_data(n, 2);
    lagrange_interpolated_data << x, lagrange_interpolated;


    std::string path = "data/quadratic_";
    std::filesystem::path paths[4] = {
        path+"data.txt",
        path+"cubic.txt", 
        path+"clamped_cubic.txt",
        path+"lagrange.txt", 
    };
    datagen<long double>::write(paths[0], data);
    datagen<long double>::write(paths[1], cubic_interpolated_data);
    datagen<long double>::write(paths[2], clamped_cubic_interpolated_data);
    datagen<long double>::write(paths[3], lagrange_interpolated_data);

    // plotter << "plot '-' with lines title 'Lagrange', '-' with lines title 'Cubic spline', '-' with points title 'Data'\n";
    std::string titles[4] = {
        "Data",
        "Cubic spline", 
        "Clamped Cubic spline", 
        "Lagrange", 
    };
    std::string styles[4] = {
        "points",
        "lines", 
        "lines", 
        "lines", 
    };
   
    Plotter<long double> plotter;
    plotter.plot(3, paths, titles, styles);
}

void test_Chebyshev() {

    int n = 30;
    int m = 2;

    std::pair<double, double> range = {-1, 1};
    Eigen::VectorX<double> params(3);
    params << 1, 10, 10;
    
    Eigen::MatrixX2<double> unidata(n, m);
    Eigen::MatrixX2<double> chebydata(n, m);

    datagen<double>::PointGeneration pg;

    pg = datagen<double>::PointGeneration::UNIFORM;
    datagen<double>::damped_cosine(unidata, params, range, pg);
    
    pg = datagen<double>::PointGeneration::CHEBYSHEV;
    datagen<double>::damped_cosine(chebydata, params, range, pg);

    // Chebyshev points don't land exactly on -1 and 1, so range changes.
    range = {chebydata.col(0).minCoeff(), chebydata.col(0).maxCoeff()};

    n = 60;
    Eigen::VectorX<double> x(n);
    datagen<double>::generate_points(x, range, pg);

    LagrangeInterpolator<double> interpolator;
    
    interpolator.fit(unidata, 1);
    Eigen::VectorX<double> uni_interpolated = interpolator((Eigen::VectorX<double>)x);
    Eigen::MatrixX2<double> uni_interpolated_data(n, 2);
    uni_interpolated_data << x, uni_interpolated;

    interpolator.fit(chebydata, 1);
    Eigen::VectorX<double> cheby_interpolated = interpolator((Eigen::VectorX<double>)x);
    Eigen::MatrixX2<double> cheby_interpolated_data(n, 2);
    cheby_interpolated_data << x, cheby_interpolated;

    std::string path = "data/chebyshev_";
    std::filesystem::path paths[4] = {
        path+"chebydata.txt",
        path+"unidata.txt",
        path+"uniform.txt",
        path+"chebyshev.txt", 
    };
    datagen<double>::write(paths[0], chebydata);
    datagen<double>::write(paths[1], unidata);
    datagen<double>::write(paths[2], uni_interpolated_data);
    datagen<double>::write(paths[3], cheby_interpolated_data);

    std::string titles[4] = {
        "Chebyshev points",
        "Uniform points",
        "Uniform Lagrange", 
        "Chebyshev Lagrange", 
    };

    std::string styles[4] = {
        "points",
        "points",
        "lines", 
        "lines", 
    };

    Plotter<double> plotter;
    plotter.plot(4, paths, titles, styles);
}

int main(int argc, char **argv) {
    // test_fft();
    // test_damped_cosine();
    test_damped_cosine_cubic();
    // test_quadtratic_splines();
    // test_cosine_cubic();
    // test_Chebyshev();
    // const std::filesystem::path DATA_PATH = std::filesystem::current_path().parent_path()/"data";
    // std::filesystem::path data_file = DATA_PATH/"random_data.txt";
    // test();
    // Eigen::VectorX<double> params(3);
    // params << 0, 0, 1;
    // datagen<double>::polynomial(data, params, range, pg);
}

// void polynomial_gen() {

// }
