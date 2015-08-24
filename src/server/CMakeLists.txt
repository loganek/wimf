SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")

add_executable(wimf-server 
  main.cpp
  server.cpp
  client.cpp
)

target_link_libraries (wimf-server wimf-protocol)