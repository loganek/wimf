cmake_minimum_required(VERSION 2.8.9)
project(wimf)
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -pedantic -std=c++11")
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wl,--no-as-needed")
SET(PROJECT_SOURCE_DIR "${wimf_SOURCE_DIR}/src")

include_directories("${wimf_BINARY_DIR}/src")

find_package(Protobuf REQUIRED)
include_directories(${PROTOBUF_INCLUDE_DIRS})

include_directories(${PROJECT_SOURCE_DIR})
add_subdirectory(src)
