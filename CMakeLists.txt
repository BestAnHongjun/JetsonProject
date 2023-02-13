cmake_minimum_required(VERSION 2.6)

project(project_name)

option(CUDA_USE_STATIC_CUDA_RUNTIME OFF)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall -Ofast -Wfatal-errors -D_MWAITXINTRIN_H_INCLUDED")
add_definitions(-O2 -pthread)

# Set output dir
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)

# Set include dir
include_directories(${PROJECT_SOURCE_DIR})
include_directories(${PROJECT_SOURCE_DIR}/include)

# Set 3rdParty packages
find_package(CUDA REQUIRED)
include_directories(/usr/local/cuda/include)
link_directories(/usr/local/cuda/lib64)
link_directories(/usr/lib/arrch64-linux-gnu)
find_package(OpenCV)
include_directories(${OpenCV_INCLUDE_DIRS})
find_package(JPEG REQUIRED)
include_directories(${JPEG_INCLUDE_DIR})
find_package(vpi REQUIRED)

add_subdirectory(module)

# ************ DEMO ****************
# add exe
# add_executable(demo demo.cpp)
# add module
# add_library(demo SHARED source/demo.cpp)
# link demo
# target_link_libraries(demo ${JPEG_LIBRARIES} -lpthread -lm) # jpeg
# target_link_libraries(demo -lpthread -lm)     # multi-threads
# target_link_libraries(demo nvinfer cudart)    # tensorrt
# target_link_libraries(demo ${OpenCV_LIBS})    # opencv
# target_link_libraries(demo vpi)               # vpi
# ************ DEMO ****************

# MainBody:

# End
