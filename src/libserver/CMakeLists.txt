# Define library. Only source files here!
cmake_minimum_required(VERSION 3.2 FATAL_ERROR)
project(libserver VERSION 0.1.0 LANGUAGES CXX)

add_library(libserver src/ServerHandler.cpp src/JsonParse.cpp)

target_include_directories(libserver PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>)

target_link_libraries(libserver 
					proxygenhttpserver
					folly
					glog
					gflags)