cmake_minimum_required(VERSION 2.6)
project(project_modules)

# Global Config
set (GLOBAL_EXAMPLE "ON")

# Timer Module
set (TIMER_SWITCH   "ON")
set (TIMER_EXAMPLE  "ON")
if (${TIMER_SWITCH} STREQUAL "ON")
    include_directories(timer)
    add_subdirectory(timer)
endif()

# PV Module
set (PV_SWITCH      "ON")
set (PV_EXAMPLE     "ON")
if (${PV_SWITCH} STREQUAL "ON")
    include_directories(pv)
    add_subdirectory(pv)
endif()

# BufQueue Module
set (BUFQUEUE_SWITCH    "ON")
set (BUFQUEUE_EXAMPLE   "ON")
if (${BUFQUEUE_SWITCH} STREQUAL "ON")
    include_directories(bufqueue)
    add_subdirectory(bufqueue)
endif()

# UDP Module
set (UDP_SWITCH    "ON")
set (UDP_EXAMPLE   "ON")
if (${UDP_SWITCH} STREQUAL "ON")
    include_directories(udp)
    add_subdirectory(udp)
endif()

# OpenCV Module
set (OPENCV_SWITCH    "ON")
set (OPENCV_EXAMPLE   "ON")
if (${OPENCV_SWITCH} STREQUAL "ON")
    include_directories(opencv)
    add_subdirectory(opencv)
endif()

# Realsense Camera Module
set (REALSENSE_SWITCH    "ON")
set (REALSENSE_EXAMPLE   "ON")
if (${REALSENSE_SWITCH} STREQUAL "ON")
    include_directories(realsense_camera)
    add_subdirectory(realsense_camera)
endif()

# Image-Trans Camera Module
set (IMAGETRANS_SWITCH    "ON")
set (IMAGETRANS_EXAMPLE   "ON")
if (${IMAGETRANS_SWITCH} STREQUAL "ON")
    include_directories(image_trans)
    add_subdirectory(image_trans)
endif()