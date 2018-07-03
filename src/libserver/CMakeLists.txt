# Define library. Only source files here!
cmake_minimum_required(VERSION 3.2 FATAL_ERROR)
project(libserver VERSION 0.1.0 LANGUAGES CXX)

add_library(libserver src/ServerHandler.cpp src/JsonParse.cpp src/TensorflowServingClient.cpp)

target_include_directories(libserver PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>)

target_link_libraries(libserver 
					proxygenhttpserver
					folly
					glog
					gflags
					gRPC::grpc++_reflection 
					protobuf 
					tensorflow_all 
					tensorflow_framework 
					prediction_service_proto 
					predict_proto 
					classification_proto 
					get_model_metadata_proto 
					get_model_status_proto 
					inference_proto 
					input_proto 
					model_management_proto 
					model_proto model_service_proto 
					prediction_log_proto 
					regression_proto session_service_proto)