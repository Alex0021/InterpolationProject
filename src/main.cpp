#include <iostream>
#include "DataReader.hpp"

int main(int argc, char **argv) {
    std::filesystem::path my_path = std::filesystem::current_path();
    DataReader::read<double>(my_path);
}

