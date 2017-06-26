cmake_minimum_required(VERSION 2.8)

project(gransac)

add_definitions(-std=c++1y -O3)

add_subdirectory(src)

add_subdirectory(examples/line)
