cmake_minimum_required(VERSION 3.10)
set(PROJECT_IMPL ${PROJECT_NAME})
project(${PROJECT_NAME}_test)

add_definitions(-std=c++11 -fpic)
SET(CMAKE_BUILD_TYPE Debug)

add_executable(${PROJECT_NAME} main.cpp)

target_include_directories(${PROJECT_NAME} PUBLIC ../include)
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_BINARY_DIR})
target_link_libraries(${PROJECT_NAME} ${PROJECT_IMPL})
