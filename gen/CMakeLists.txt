add_custom_target(GEN
        COMMAND echo "Generating google protobuf c++ file..."
        COMMAND protoc -I=${PROJECT_SOURCE_DIR}/protofile --cpp_out=${PROJECT_SOURCE_DIR}/gen ${PROJECT_SOURCE_DIR}/protofile/*.proto
)

aux_source_directory(./ SRC)
add_library(libgen-proto ${SRC})
add_dependencies(libgen-proto GEN)
target_link_libraries(libgen-proto protobuf)