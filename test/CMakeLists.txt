cmake_minimum_required(VERSION 2.8.3)
project(test_opencl)

add_compile_options(-std=c++11)

find_package(OpenCL REQUIRED)
include_directories(${OpenCL_INCLUDE_DIRS_})
link_directories(${OpenCL_LIBRARY})

add_executable(test_cl.o ../src/cl_wrapper.cpp test.cpp)
target_link_libraries(test_cl.o ${OpenCL_LIBRARY})
