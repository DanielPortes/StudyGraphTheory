cmake_minimum_required(VERSION 3.16)
project(graphs2022)

set(CMAKE_CXX_STANDARD 17)
# set(CMAKE_CXX_EXTENSIONS OFF)
include_directories(src)

message(NOTICE "\n\tDesenvolvido por: Daniel Fagundes - 100% \n\n" ...)
add_executable(execGrupo5
        src/Grafo.cpp
        src/Grafo.h
        src/main.cpp
        src/Timer.h
        src/DisjointSet.h)

add_executable(execDanielFagundes
        src/Grafo.cpp
        src/Grafo.h
        src/main.cpp
        src/Timer.h
        src/DisjointSet.h)
