cmake_minimum_required(VERSION 3.27)
project(own_db_on_C C)

set(CMAKE_C_STANDARD 11)

add_executable(own_db_on_C
        main.c
        headers/students.h
)