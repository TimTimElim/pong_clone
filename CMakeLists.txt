cmake_minimum_required(VERSION 3.31)
project(pong_clone)

set(CMAKE_CXX_STANDARD 20)

find_package(SFML COMPONENTS Network Graphics Window Audio System CONFIG REQUIRED)
add_executable(pong_clone main.cpp)
target_link_libraries(pong_clone PRIVATE SFML::Network SFML::Graphics SFML::Window SFML::Audio SFML::System)

