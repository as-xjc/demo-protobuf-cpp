cmake_minimum_required(VERSION 2.8)
project(proto)

set(CMAKE_CXX_STANDARD 11)

link_directories(/usr/local/lib)

set(SOURCE_FILES main.cpp)

add_subdirectory(gen)
add_executable(proto ${SOURCE_FILES})
target_link_libraries(proto libgen-proto)