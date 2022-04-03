cmake_minimum_required(VERSION 3.21)
project(article_management_1.0 C)

set(CMAKE_C_STANDARD 99)

add_executable(article_management_1.0 main.c store_functions.c store_functions.h general_functions.c general_functions.h)
