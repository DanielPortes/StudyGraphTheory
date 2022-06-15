cmake_minimum_required(VERSION 3.16)
project(dev)

set(CMAKE_CXX_STANDARD 14)

include_directories(src)

add_executable(dev
        src/Grafo.cpp
        src/Grafo.h
        src/main.cpp
        src/parametros.h)
