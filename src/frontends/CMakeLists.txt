cmake_minimum_required(VERSION 3.2 FATAL_ERROR)

add_executable(frontend_server src/frontend_server_main.cpp)

target_link_libraries(frontend_server libserver)