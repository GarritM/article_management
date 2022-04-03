cmake_minimum_required(VERSION 3.21)
project(article_management_1.1 C)

set(CMAKE_C_STANDARD 99)

add_executable(article_management_1.1 main.c user_interface.c user_interface.h file_functions.c file_functions.h editing_functions.c editing_functions.h)
