cmake_minimum_required(VERSION 3.12)
project(walk_test C)

set(CMAKE_C_STANDARD 99)

add_executable(walk_test main.c
        huro.c
        huro.h
        ./library/
        ./mission/)