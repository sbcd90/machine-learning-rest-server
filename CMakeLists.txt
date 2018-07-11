cmake_minimum_required(VERSION 3.3 FATAL_ERROR)

### Hunter snapshot that will be used ###
include("cmake/HunterGate.cmake")

HunterGate(
  URL "https://github.com/ruslo/hunter/archive/v0.19.72.tar.gz"
  SHA1 "3ed5ea39abf52afab06c77e1b7d7e41e939752f1"  
  FILEPATH "${CMAKE_CURRENT_LIST_DIR}/cmake/Hunter/config.cmake"    
)

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

# Include Boost as an imported target
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_LIBS ON)
find_package(Boost 1.58.0 REQUIRED COMPONENTS thread system chrono date_time atomic)
add_library(boost INTERFACE IMPORTED)
set_property(TARGET boost PROPERTY
    INTERFACE_INCLUDE_DIRECTORIES ${Boost_INCLUDE_DIRS})
set_property(TARGET boost PROPERTY
    INTERFACE_LINK_LIBRARIES ${Boost_LIBRARIES})

set(XGBOOSTCPP_ROOT_DIR "${CMAKE_CURRENT_LIST_DIR}")

# make sure these are before first hunter_add_package() call
option(XGBOOSTER_BUILD_TESTS "Build tests" ON)
option(XGBOOSTER_BUILD_APPS "Build applications" ON)
option(XGBOOSTER_SERIALIZE_WITH_CEREAL "Serialize w/ cereal" ON) # hunter update
option(XGBOOSTER_DO_LEAN "Build for prediction only (size reduction)" OFF)

###################
## Dependencies ###
###################

# cereal
find_package(cereal REQUIRED)

find_package(OpenMP)
if(OPENMP_FOUND)
  # Set the globally for now (no clean cmake way to propagate OpenMP_EXE_LINKER_FLAGS
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
  set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${OpenMP_EXE_LINKER_FLAGS}")
endif()

subdirs(src)
