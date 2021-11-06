cmake_minimum_required(VERSION 3.10)
project(story_teller)

add_definitions(-std=c++11 -fpic)
SET(CMAKE_BUILD_TYPE Debug)

find_package(Qt5 REQUIRED COMPONENTS Core Gui Qml Quick Svg Multimedia Network Sql)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
option(BUILD_TESTS "build tests" OFF)

file(GLOB_RECURSE SOURCES ${CMAKE_SOURCE_DIR}/src src/app/*.cpp src/domain/*.cpp src/presentation/*.cpp src/*.qrc include/*.hpp)

# if(ANDROID)
#     add_subdirectory(android)
# else(ANDROID)
    add_library(${PROJECT_NAME} SHARED "")

    # Executable target
    add_executable(${PROJECT_NAME}-bin src/main.cpp)
    set_target_properties(${PROJECT_NAME}-bin PROPERTIES OUTPUT_NAME ${PROJECT_NAME})
    target_include_directories(${PROJECT_NAME}-bin PUBLIC include)
    target_link_libraries(${PROJECT_NAME}-bin ${PROJECT_NAME})
# endif()

target_include_directories(${PROJECT_NAME} PUBLIC include)
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_BINARY_DIR})

target_sources(${PROJECT_NAME} PRIVATE ${SOURCES})

target_link_libraries(${PROJECT_NAME} ${LIBS})
qt5_use_modules(${PROJECT_NAME} Core Qml Quick Svg Multimedia Network Sql)
