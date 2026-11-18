cmake_minimum_required(VERSION 4.3)
project(Heap)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(Heap main.cpp)

target_sources(Heap
        PRIVATE
            FILE_SET CXX_MODULES FILES
                my_heap.cppm
)