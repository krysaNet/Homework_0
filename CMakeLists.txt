cmake_minimum_required(VERSION 4.3)
project(Task_20261114)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(Task_20261114 task_20261114.cpp)

target_sources(Task_20261114
        PRIVATE
        FILE_SET CXX_MODULES FILES
        my_heap.cppm
)