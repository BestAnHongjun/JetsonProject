cmake_minimum_required(VERSION 2.6)
project(udp_modules)

add_library(udp SHARED udp_sender.cpp udp_receiver.cpp)
target_link_libraries(udp pthread bufqueue)

if (${GLOBAL_EXAMPLE} STREQUAL "ON" AND ${UDP_EXAMPLE} STREQUAL "ON")
    add_executable(udp_sender_example udp_sender_example.cpp)
    target_link_libraries(udp_sender_example udp)

    add_executable(udp_receiver_example udp_receiver_example.cpp)
    target_link_libraries(udp_receiver_example udp)

    add_executable(udp_receiver_asynch_example udp_receiver_asynch_example.cpp)
    target_link_libraries(udp_receiver_asynch_example udp)
endif()

set (UDPLIB udp)