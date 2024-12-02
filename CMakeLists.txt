cmake_minimum_required(VERSION 3.31)
project(aoc_2024_c C)

set(CMAKE_C_STANDARD 23)

add_executable(aoc_2024_c day1.c
        main.c
        day2.c
        aoc.h)
