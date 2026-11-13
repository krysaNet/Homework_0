cmake_minimum_required(VERSION 4.3)
project(Trie)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(Trie main.cpp)

target_sources(Trie
        PRIVATE
            FILE_SET CXX_MODULES FILES
                my_trie.cppm
)