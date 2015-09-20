execute_process(COMMAND protoc -I${CMAKE_CURRENT_SOURCE_DIR}/app/src/main/java/ -I${CMAKE_CURRENT_SOURCE_DIR}/../protocol/ --java_out=${CMAKE_CURRENT_SOURCE_DIR}/app/src/main/java/  ${CMAKE_CURRENT_SOURCE_DIR}/../protocol/wimf.proto
)
