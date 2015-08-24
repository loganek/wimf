SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")

add_subdirectory (data_frames)

add_library(wimf-protocol 
  protocol.cpp
  databuffer.cpp
  logger.cpp
)

target_link_libraries (wimf-protocol wimf-data-frames)