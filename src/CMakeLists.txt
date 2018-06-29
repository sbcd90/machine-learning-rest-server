cmake_minimum_required(VERSION 3.2 FATAL_ERROR)
project("tf_serving_rest_server")

# Targets that we develop
add_subdirectory(libserver)
add_subdirectory(frontends)