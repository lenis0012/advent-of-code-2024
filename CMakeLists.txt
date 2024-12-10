cmake_minimum_required(VERSION 3.31)
project(aoc_2024_c C)

set(CMAKE_C_STANDARD 23)

if(${CMAKE_BUILD_TYPE} MATCHES Debug)
    link_libraries(asan)
endif ()

file(GLOB week1_src week1/*.c)

include_directories(.)

add_executable(aoc_2024_c
        main.c
        ${week1_src}
        week2/day8.c
        week2/day9.c
        aoc.c)

target_link_libraries(aoc_2024_c m)
