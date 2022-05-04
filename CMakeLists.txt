cmake_minimum_required(VERSION 3.22)
project(teste)

set(CMAKE_CXX_STANDARD 14)

include_directories(src)

add_executable(teste
        src/Grafo.cpp
        src/Grafo.h
        src/main.cpp)
