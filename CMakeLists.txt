cmake_minimum_required(VERSION 3.10)
project(Ex3 C)

set(CMAKE_C_STANDARD 99)

add_executable(Ex3 main.c team.h driver.h season.h driver.c team.c)