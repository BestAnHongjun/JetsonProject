cmake_minimum_required(VERSION 2.6)
project(opencv_modules)

find_package(OpenCV)
include_directories(${OpenCV_INCLUDE_DIRS})

add_library(opencv opencv.cpp)
target_link_libraries(opencv ${OpenCV_LIBS})

if (${GLOBAL_EXAMPLE} STREQUAL "ON" AND ${OPENCV_EXAMPLE} STREQUAL "ON")
    add_executable(opencv_example opencv_example.cpp)
    target_link_libraries(opencv_example opencv)
endif()

set (OPENCVLIB opencv)