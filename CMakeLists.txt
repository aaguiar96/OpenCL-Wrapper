cmake_minimum_required(VERSION 2.8.3)
project(cl_wrapper)

#add_definitions(-DDEBUG)

find_package(OpenCL REQUIRED)
find_package(catkin REQUIRED COMPONENTS cmake_modules)

include_directories(
  /include
  ${catkin_INCLUDE_DIRS}
  ${OpenCL_INCLUDE_DIRS}
)

catkin_package(
  INCLUDE_DIRS include
  LIBRARIES cl_wrapper
  CATKIN_DEPENDS 
  DEPENDS OpenCL
)

add_library(cl_wrapper src/cl_wrapper.cpp)

install(TARGETS cl_wrapper
  ARCHIVE DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
  LIBRARY DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
  RUNTIME DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION}
)

install(DIRECTORY include/${PROJECT_NAME}/
  DESTINATION ${CATKIN_PACKAGE_INCLUDE_DESTINATION}
  FILES_MATCHING PATTERN "*.hpp"
)
