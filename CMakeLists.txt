cmake_minimum_required(VERSION 3.21)
project(article_management_1.2 C)

set(CMAKE_C_STANDARD 99)
link_libraries(ws2_32 wsock32)

add_executable(article_management_1.2 main.c user_interface.c user_interface.h file_functions.c file_functions.h editing_functions.c editing_functions.h network_functions.c network_functions.h)
