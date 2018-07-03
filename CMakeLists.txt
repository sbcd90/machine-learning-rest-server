cmake_minimum_required(VERSION 3.3 FATAL_ERROR)
project("tf_serving_rest_server" VERSION 0.1.0 LANGUAGES CXX)
set(CMAKE_PREFIX_PATH "/usr/local/lib")
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake/sanitizers-cmake")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -pedantic -Wextra -Wno-deprecated-declarations -pthread")
if (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-unsupported-friend -pthread")
endif()

set(CMAKE_EXPORT_COMPILE_COMMANDS 1)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED on)

set(CMAKE_CXX_EXTENSIONS off)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED on)

find_package(Proxygen REQUIRED)
find_package(Json REQUIRED)
find_package(GRPC REQUIRED)
find_package(Protobuf REQUIRED)
find_package(TensorFlow REQUIRED)

subdirs(src)
