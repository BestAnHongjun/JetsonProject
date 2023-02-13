cmake_minimum_required(VERSION 2.6)
project(bufqueue_modules)

add_library(bufqueue bufqueue.cpp)
target_link_libraries(bufqueue)

if (${GLOBAL_EXAMPLE} STREQUAL "ON" AND ${BUFQUEUE_EXAMPLE} STREQUAL "ON")
    add_executable(bufqueue_example bufqueue_example.cpp)
    target_link_libraries(bufqueue_example pthread bufqueue)
endif()

set (BUFQUEUELIB pthread bufqueue)