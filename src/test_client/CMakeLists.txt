SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")

add_executable(wimf-test-client 
  main.cpp
)

target_link_libraries(wimf-test-client wimf-protocol)