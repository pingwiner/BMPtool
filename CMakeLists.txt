cmake_minimum_required(VERSION 3.15)
project(demo)

set(CMAKE_CXX_STANDARD 14)

add_executable(demo main.cpp BmpFile.cpp BmpFile.h Converter.cpp Converter.h)