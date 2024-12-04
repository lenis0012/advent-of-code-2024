cmake_minimum_required(VERSION 3.31)
project(aoc_2024_c C)

set(CMAKE_C_STANDARD 23)

add_executable(aoc_2024_c day1.c
        main.c
        day2.c
        day3.c
        day4.c
        aoc.h)

if(${CMAKE_BUILD_TYPE} MATCHES Debug)
    target_link_libraries(aoc_2024_c PUBLIC asan)
endif ()
