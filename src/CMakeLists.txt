cmake_minimum_required(VERSION 3.2 FATAL_ERROR)
project("tf_serving_rest_server")
include(CheckCCompilerFlag)

check_c_compiler_flag("-Wl,-dead_strip" FLAG_dead_strip)
if(NOT FLAG_dead_strip)
  set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -Wl,--gc-sections")
  set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -Wl,--gc-sections,--icf=safe")
else()
  set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -Wl,-dead_strip")
  set(CMAKE_EXE_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -Wl,-dead_strip")
endif()

###############
### xgboost ###
###############

#hunter_add_package(xgboost)
find_package(xgboost REQUIRED)

###############
### cxxopts ### :: std::regex >= gcc 4.8
###############

find_package(cxxopts REQUIRED)

# Targets that we develop
add_subdirectory(libserver)
add_subdirectory(frontends)