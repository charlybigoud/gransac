cmake_minimum_required(VERSION 2.8)

project(line)

add_executable(line src/line.cpp)

target_link_libraries(line gransac)
