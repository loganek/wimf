PROTOBUF_GENERATE_CPP(ProtoSources ProtoHeaders wimf.proto)

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")

add_library(wimf-protocol STATIC logger.cpp protocol.cpp ${ProtoSources} ${ProtoHeaders})

target_link_libraries(wimf-protocol ${PROTOBUF_LIBRARY})
