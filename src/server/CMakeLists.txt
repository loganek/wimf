SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")

add_subdirectory (data_frames)

add_executable(wimf-server 
  main.cpp
  server.cpp
  client.cpp
  logger.cpp
  protocol.cpp
)

target_link_libraries (wimf-server wimf-data-frames)