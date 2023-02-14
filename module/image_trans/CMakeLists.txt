cmake_minimum_required(VERSION 2.6)
project(image_trans_modules)

add_library(image_trans SHARED image_sender.cpp image_receiver.cpp)
target_link_libraries(image_trans pthread opencv udp)

if (${GLOBAL_EXAMPLE} STREQUAL "ON" AND ${IMAGETRANS_EXAMPLE} STREQUAL "ON")
    add_executable(image_sender_example image_sender_example.cpp)
    target_link_libraries(image_sender_example image_trans)

    add_executable(image_receiver_example image_receiver_example.cpp)
    target_link_libraries(image_receiver_example image_trans)
endif()

set (IMAGETRANSLIB image_trans)