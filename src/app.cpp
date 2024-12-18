#include <iostream>
#include "data_reader.hpp"
#include "lagrange_interpolator.hpp"
#include "barycentric_interpolator.hpp"
#include "cubic_spline_interpolator.hpp"
#include "plotter.hpp"
#include "boost/program_options.hpp"
#include <memory>
#include "datagen.hpp"

#define DEFAULT_DATAFOLDER_PATH "./datapoints/"
#define DEFAULT_DATAFILE_PATH "./datapoints/default.txt"
#define OUTPUT_FOLDER "./output/"
#define DEFAULT_NUM_POINTS 1000

void interpolate_data_from_file(const std::filesystem::path& filepath);
void generate_plot_with_factory(const std::filesystem::path& filepath, const std::string& interpolation_scheme, std::vector<std::string>& options, const int fitting_dim, int n_samples);
void test_damped_cosine_cubic();
void test_Chebyshev();

template <typename T>
void plot_multiple_interpolators(const std::filesystem::path& filepath, const std::vector<std::string>& interpolators, int n_samples, std::vector<std::string>& extra_params)
{
    Eigen::MatrixX<T> X = DataReader<T>::read(filepath);

    std::vector<std::unique_ptr<Interpolator<T>>> interpolator_objects;
    for (const auto& interpolator : interpolators)
    {
        interpolator_objects.push_back(DataReader<T>::interpolator_from_file(filepath, interpolator, extra_params, 1));
    }

    // Generate query points
    Eigen::MatrixX<T> X_inter(n_samples,2);
    X_inter.col(0) = Eigen::VectorX<T>::LinSpaced(n_samples,X.col(0).minCoeff(),X.col(0).maxCoeff());

    // Configure the plot
    std::filesystem::path paths[interpolators.size()+1];
    paths[0] = filepath;
    datagen<T>::write(paths[0], X);
    for (int i = 1; i < interpolators.size()+1; i++)
    {
       paths[i] = OUTPUT_FOLDER + interpolators[i-1] + "_interpolated.txt";
        X_inter.col(1) = (*interpolator_objects[i-1])(X_inter.col(0));
        datagen<T>::write(paths[i], X_inter);
    }

    std::string titles[interpolators.size()+1];
    titles[0] = "Data";
    for (int i = 1; i < interpolators.size()+1; i++)
    {
        titles[i] = interpolators[i-1];
    }

    std::string styles[interpolators.size()+1];
    styles[0] = "points";
    for (int i = 1; i < interpolators.size()+1; i++)
    {
        styles[i] = "lines";
    }

    // Plot the lines
    Plotter<T> harry_plotter;
    harry_plotter.plot(interpolators.size()+1, paths, titles, styles);

    // Clear reserved interpolator memory
    for (int i = 0; i < interpolators.size(); i++)
    {
        interpolator_objects[i].release();
    }
}

template void plot_multiple_interpolators<double>(const std::filesystem::path& filepath, const std::vector<std::string>& interpolators, int n_samples, std::vector<std::string>& extra_params);
template void plot_multiple_interpolators<float>(const std::filesystem::path& filepath, const std::vector<std::string>& interpolators, int n_samples, std::vector<std::string>& extra_params);
template void plot_multiple_interpolators<int>(const std::filesystem::path& filepath, const std::vector<std::string>& interpolators, int n_samples, std::vector<std::string>& extra_params);

void generate_plot_with_factory(const std::filesystem::path& filepath, const std::string& interpolation_scheme, std::vector<std::string>& options, const int fitting_dim, int n_samples)
{
    auto interpolator = DataReader<double>::interpolator_from_file(filepath, interpolation_scheme, options, fitting_dim);
    Eigen::MatrixX<double> X = DataReader<double>::read(filepath);

    // Generate query points
    Eigen::MatrixX2d X_inter(n_samples,2);
    X_inter.col(0) = Eigen::VectorX<double>::LinSpaced(n_samples,X.col(0).minCoeff(),X.col(0).maxCoeff());
    X_inter.col(1) = (*interpolator)(X_inter.col(0));

    // Configure the plot
    std::string path = OUTPUT_FOLDER;
    std::filesystem::path paths[2] = {path+"default_data.txt", path+"default_interpolated.txt"};
    datagen<double>::write(paths[0], X);    
    datagen<double>::write(paths[1], X_inter);
    std::string titles[2] = {"data", "interpolated"};
    std::string styles[2] = {"points", "lines"};

    // Plot the lines
    Plotter<double> harry_plotter;
    harry_plotter.plot(2, paths, titles, styles);

    // Clear reserved interpolator memory
    interpolator.release();
}

int main(int argc, char **argv) {

    // Read command line arguments using boost::program_options
    boost::program_options::variables_map vmap;
    boost::program_options::options_description desc("Allowed options");
    try {
        desc.add_options()("help", "show this help message")
                        ("file", boost::program_options::value<std::string>(), "interpolation data file (relative path)")
                        ("lagrange", "use Lagrange interpolation")
                        ("barycentric", "use barycentric interpolation")
                        ("cubic_spline", boost::program_options::value<std::vector<std::string>>()->multitoken(), 
                        "cubic spline interpolation with boundary conditions [NATURAL, NOT_A_KNOT, CLAMPED]")
                        ("samples", boost::program_options::value<int>(), "number of samples to generate");

        boost::program_options::store(boost::program_options::parse_command_line(argc, argv, desc, boost::program_options::command_line_style::unix_style ^  boost::program_options::command_line_style::allow_short), vmap);
        boost::program_options::notify(vmap);
    } catch (std::exception& e) {
        std::cout << e.what() << std::endl;
        return -1;
    } catch (...) {
        std::cout << "Unknown exception occured while parsing arguments!" << std::endl;
        return -1;
    }

    if (vmap.count("help"))
    {
        std::cout << desc << std::endl;
        return 0;
    }

    // Create output folder if needed
    if (!std::filesystem::exists(OUTPUT_FOLDER))
    {
        std::filesystem::create_directory(OUTPUT_FOLDER);
    }

    std::filesystem::path data_path = std::filesystem::current_path().concat("/").concat(DEFAULT_DATAFILE_PATH);
    if (vmap.count("file")) {
        data_path = std::filesystem::current_path().concat("/").concat(vmap["file"].as<std::string>());
    }

    int num_samples = DEFAULT_NUM_POINTS;
    if (vmap.count("samples")) {
        num_samples = vmap["samples"].as<int>();
    }

    // Group interpolators into vector
    std::vector<std::string> interpolators;
    if (vmap.count("lagrange")) { interpolators.push_back("lagrange"); }
    if (vmap.count("barycentric")) { interpolators.push_back("barycentric"); }
    if (vmap.count("cubic_spline")) { interpolators.push_back("cubic_spline"); }

    if (interpolators.size() > 0) {
        std::vector<std::string> extra_params = {};
        if (vmap.count("cubic_spline") > 0) {
            extra_params = vmap["cubic_spline"].as<std::vector<std::string>>();
        }
        plot_multiple_interpolators<double>(data_path, interpolators, num_samples, extra_params);
        return 0;
    }

    if (vmap.empty())
    {
        std::vector<std::string> options= {};
        generate_plot_with_factory(data_path, "lagrange", options, 1, num_samples);
        return 0;
    }
}

