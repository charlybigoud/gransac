cmake_minimum_required(VERSION 2.8)

add_library(gransac
    ransac.cpp
)

target_include_directories(gransac PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
