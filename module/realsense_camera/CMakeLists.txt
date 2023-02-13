cmake_minimum_required(VERSION 2.6)
project(realsense_camera_modules)

add_library(realsense_camera SHARED realsense_camera.cpp)
target_link_libraries(realsense_camera realsense2)

if (${GLOBAL_EXAMPLE} STREQUAL "ON" AND ${REALSENSE_EXAMPLE} STREQUAL "ON")
    add_executable(realsense_camera_example realsense_camera_example.cpp)
    target_link_libraries(realsense_camera_example realsense_camera)
endif()

set (REALSENSELIB realsense_camera)