
cmake_minimum_required (VERSION 3.0)
project (Interpolation-Project)

set(CMAKE_CXX_STANDARD 20)

set(SOURCES "src/data_reader.cpp"
            "src/polynomial_interpolator.cpp" 
            "src/datagen.cpp" 
            "src/lagrange_interpolator.cpp"
            "src/barycentric_interpolator.cpp"
            "src/cubic_spline_interpolator.cpp"
            "src/chebychev_interpolator.cpp"
            "src/plotter.cpp"
            "src/fourier_approximator.cpp"
)

set(TEST_SOURCES "test/test_polynomial_interpolator.cpp"
                 "test/test_lagrange_interpolator.cpp"
                 "test/test_barycentric_interpolator.cpp"
                 "test/test_cubic_spline_interpolator.cpp")

# Make sure git submodules are loaded #
set(GIT_EXECUTABLE "git")    
option(GIT_SUBMODULE "Check submodules during build" ON)
if(GIT_SUBMODULE)
execute_process(COMMAND ${GIT_EXECUTABLE} submodule update --init --recursive WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} RESULT_VARIABLE GIT_SUBMOD_RESULT)
if(NOT GIT_SUBMOD_RESULT EQUAL "0")
message(FATAL_ERROR "git submodule update --init --recursive failed with ${GIT_SUBMOD_RESULT}, please checkout submodules")
endif()
message(STATUS "Submodule update done")
endif(GIT_SUBMODULE)

# Add subfolders to project (with CMakeLists.txt file)
add_subdirectory(external/googletest)

# Add include_directories for compilation (header & lib files)
include_directories(external/eigen include/ external/gnu-iostream)

# Define targets
add_executable(main "src/main.cpp" ${SOURCES})

# Define tests
add_executable(tests ${TEST_SOURCES} ${SOURCES})
target_link_libraries(tests gtest_main gtest pthread Boost::iostreams Boost::system Boost::filesystem)

add_custom_target(test ./tests DEPENDS tests)

#GNU PLOT
#find dependencies (Boost)
find_package(Boost REQUIRED COMPONENTS iostreams system filesystem program_options)
#adds the search path for includes
include_directories(external/gnuplot-iostream)
#adds linked libraries to produce executable
target_link_libraries(main PRIVATE
Boost::iostreams
Boost::system
Boost::filesystem
Boost::program_options
)


# Doxygen
option(GENERATE_DOC "Generate documentation with Doxygen" ON)
if(GENERATE_DOC)
    find_package(Doxygen)
    if(DOXYGEN_FOUND)
        set(DOXYGEN_OUT ${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile)
        add_custom_target(doc
        COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_OUT}
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        COMMENT "Generating API documentation with Doxygen"
        VERBATIM
        )
        message(STATUS "Doxygen documentation done")
    else(DOXYGEN_FOUND)
        message("Doxygen need to be installed to generate the doxygen documentation")
    endif(DOXYGEN_FOUND)
endif(GENERATE_DOC)