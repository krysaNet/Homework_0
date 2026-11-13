cmake_minimum_required(VERSION 4.3)
project(BST)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(BST main.cpp)

target_sources(BST
        PRIVATE
            FILE_SET CXX_MODULES FILES
                my_set.cppm
)