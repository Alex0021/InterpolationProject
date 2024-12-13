# Interpolation Project

## Overview

This project provides various interpolation methods and utilities. It includes custom exception handling, polynomial interpolators, and data readers.

## Prerequisites

- C++20 or later
- CMake 3.10 or later
- Doxygen 1.8.20 or later
- Boost 1.75.0 or later

Make sure these dependencies are satisfied before building the project!

**Be carefull** with the Doxygen used for the documentation, it requires the `dot` package from `graphviz` in order to display
class dependency diagrams on the html doc website.

Furthurmore, some equations have been added to some functions descriptions, which requires a **full** install of `latex`. 
However, this **will not** cause documentatin to fail generating.

To install `graphviz`
```sh
sudo apt-get install graphviz
```

To install `latex` with all required dependencies:
```sh
sudo apt-get install texlive-full
```


## Compilation

To compile the project, follow these steps:

1. Clone the repository:
    ```sh
    git clone https://github.com/your-repo/InterpolationProject.git
    cd InterpolationProject
    ```

2. Create a build directory and navigate into it:
    ```sh
    mkdir build
    cd build
    ```

3. Run CMake to configure the project:
    ```sh
    cmake ..
    ```

4. Build the project:
    ```sh
    make
    ```

## Running the Project



To run the project, execute the following command from the build directory (everything end up to default):
```sh
./InterpolationProject
```

The project supports various command line arguments to plot data from files using different interpolator types. Below are the available options:

- `--help`: Show the help message.
- `--file <path>`: Specify the interpolation data file (relative path).
- `--lagrange`: Use Lagrange interpolation.
- `--barycentric`: Use barycentric interpolation.
- `--cubic_spline [NATURAL, CLAMPED]`: Use cubic spline interpolation with specified boundary conditions.

### Example Usage

To plot data using Lagrange interpolation:
```sh
./InterpolationProject --file datapoints/default.txt --lagrange
```

To plot data using cubic spline interpolation with natural boundary conditions:
```sh
./InterpolationProject --file datapoints/default.txt --cubic_spline natural
```

To plot data using multiple interpolators:
```sh
./InterpolationProject --file datapoints/default.txt --lagrange --barycentric --cubic_spline natural
```

## Running Tests

To run the tests, execute the following command from the build directory:
```sh
make test
```

Alternatively, you can run the test executable directly:
```sh
./tests/InterpolationProjectTests
```

## Documentation

The project documentation is available in the `docs` directory. Key documents include:

- [Advanced GoogleTest Topics](external/googletest/docs/advanced.md)

## Custom Exceptions

The project includes a set of custom exceptions defined in `include/project_exceptions.hpp`. These exceptions handle various error cases such as index out of bounds, size mismatch, and division by zero.

## Contributing

Contributions are welcome! Please fork the repository and submit a pull request with your changes.

## License

This project is licensed under the MIT License. See the `LICENSE` file for details.

## Git submodules setup

Make sure to add the required submodules to your git repository (local):
- googletest
- eigen

The following steps will add the required submodules in the git index:
```bash
mkdir external
cd external

~/external$ git submodule add https://gitlab.com/libeigen/eigen external/eigen
~/external$ git submodule add https://github.com/google/googletest.git external/googletest
```

