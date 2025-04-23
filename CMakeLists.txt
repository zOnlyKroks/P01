cmake_minimum_required(VERSION 3.30)
project(P01)

set(CMAKE_CXX_STANDARD 26)

file(GLOB MAP_FILES "${CMAKE_CURRENT_SOURCE_DIR}/*.CSV")
file(COPY ${MAP_FILES} DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

add_executable(P01 main.cpp
        CMapIterator.h
        CGridtypeMap.h
        CMapIterator.cpp
        "CGridtypeMap.cpp")
