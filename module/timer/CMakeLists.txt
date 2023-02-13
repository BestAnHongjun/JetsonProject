cmake_minimum_required(VERSION 2.6)
project(timer_modules)

if (${GLOBAL_EXAMPLE} STREQUAL "ON" AND ${TIMER_EXAMPLE} STREQUAL "ON")
    add_executable(timer_example timer_example.cpp)
endif()
