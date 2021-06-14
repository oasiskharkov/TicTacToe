cmake_minimum_required(VERSION 2.8)

project(TicTacToe)

set(CMAKE_CXX_STANDARD 17)

file(GLOB SOURCES src/*.cpp)
file(GLOB HEADERS include/*.h)

add_executable(${CMAKE_PROJECT_NAME} ${SOURCES} ${HEADERS})

target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE include)

set_target_properties(${CMAKE_PROJECT_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/bin")
