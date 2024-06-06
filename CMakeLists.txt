cmake_minimum_required(VERSION 3.5)

project(Partition)

# SET(CMAKE_BUILD_TYPE DEBUG)
# add_compile_options(-std=c++11 -pthread -g  -ggdb -O0 -Wall   -fstack-protector-all -march=native -fnon-call-exceptions)

set(SOURCES 
    src/FMPartition.cpp
    src/parser.cpp
    src/main.cpp
)

add_executable(${PROJECT_NAME} ${SOURCES})

target_include_directories(${PROJECT_NAME}
    PRIVATE
        ${PROJECT_SOURCE_DIR}/include
)