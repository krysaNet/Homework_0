cmake_minimum_required(VERSION 4.3)
project(Task_20261114_2)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(Task_20261114_2 task_20261114_2.cpp)

target_sources(Task_20261114_2
        PRIVATE
            FILE_SET CXX_MODULES FILES
                my_stack.cppm
)