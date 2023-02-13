cmake_minimum_required(VERSION 2.6)
project(pv_modules)

if (${GLOBAL_EXAMPLE} STREQUAL "ON" AND ${PV_EXAMPLE} STREQUAL "ON")
    add_executable(pv_example pv_example.cpp)
    target_link_libraries(pv_example pthread)
endif()

set (PVLIB pthread)