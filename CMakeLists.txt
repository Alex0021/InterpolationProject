
cmake_minimum_required (VERSION 3.0)
project (Interpolation-Project)

set(CMAKE_CXX_STANDARD 20)

set(SOURCES "src/main.cpp" "src/data_reader.cpp" "src/polynomial_interpolator.cpp")

# Make sure git submodules are loaded #
set(GIT_EXECUTABLE "git")    
option(GIT_SUBMODULE "Check submodules during build" ON)
if(GIT_SUBMODULE)
  message(STATUS "Submodule update")
  execute_process(COMMAND ${GIT_EXECUTABLE} submodule update --init --recursive WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} RESULT_VARIABLE GIT_SUBMOD_RESULT)
  if(NOT GIT_SUBMOD_RESULT EQUAL "0")
    message(FATAL_ERROR "git submodule update --init --recursive failed with ${GIT_SUBMOD_RESULT}, please checkout submodules")
  endif()
endif(GIT_SUBMODULE)

# Add subfolders to project (with CMakeLists.txt file)
add_subdirectory(external/googletest)

# Add include_directories for compilation (header & lib files)
include_directories(external/eigen include/)

# Define targets
add_executable(main ${SOURCES})

# Define tests
add_executable(test_pcsc test/test.cpp)
target_link_libraries(test_pcsc gtest_main gtest pthread)

add_custom_target(test ./test_pcsc DEPENDS test_pcsc)
