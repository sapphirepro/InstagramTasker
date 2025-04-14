cmake_minimum_required(VERSION 3.30)
project(InstagramTasker)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -s -O2")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -s -O2")

add_executable(InstagramTasker main.cpp)
