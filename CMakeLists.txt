cmake_minimum_required(VERSION 4.3)
project(List)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(Project main.cpp)

target_sources(Project
        PRIVATE
            FILE_SET CXX_MODULES FILES
                my_list.cppm
)