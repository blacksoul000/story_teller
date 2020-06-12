cmake_minimum_required(VERSION 3.10)
project(story_teller)

add_definitions(-std=c++11 -fpic)
SET(CMAKE_BUILD_TYPE Debug)

if(ANDROID)
#    set (QT_ANDROID_ANT /usr/bin/ant)
    set (PYTHON_EXECUTABLE /usr/bin/python)
    set (ANDROID_COMPILER_VERSION 4.9)
endif()

find_package(Qt5 REQUIRED COMPONENTS Core Gui Qml Quick Svg Multimedia Network Sql)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

file(GLOB_RECURSE SOURCES ${CMAKE_SOURCE_DIR}/src src/app/*.cpp src/domain/*.cpp src/presentation/*.cpp src/*.qrc include/*.hpp)

if(ANDROID)
    find_package(Qt5AndroidExtras)
    
    SET(ANDROID_TARGET_ARCH ${ANDROID_ABI})
    SET(QT_VERSION ${Qt5Core_VERSION})
    include(/home/user/workspace/android/openssl/android_openssl/CMakeLists.txt)
    message("${ANDROID_EXTRA_LIBS}")
    
    add_library(${PROJECT_NAME} SHARED "")
    set_target_properties(${PROJECT_NAME} PROPERTIES VERSION 0.2)
    target_link_libraries(${PROJECT_NAME} Qt5::AndroidExtras android log)

else(ANDROID)
    add_library(${PROJECT_NAME} SHARED "")

    # Executable target
    add_executable(${PROJECT_NAME}-bin src/main.cpp)
    set_target_properties(${PROJECT_NAME}-bin PROPERTIES OUTPUT_NAME ${PROJECT_NAME})
    target_include_directories(${PROJECT_NAME}-bin PUBLIC include)
    target_link_libraries(${PROJECT_NAME}-bin ${PROJECT_NAME})
endif()

target_include_directories(${PROJECT_NAME} PUBLIC include)
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_BINARY_DIR})

target_sources(${PROJECT_NAME} PRIVATE ${SOURCES})

target_link_libraries(${PROJECT_NAME} ${LIBS})
qt5_use_modules(${PROJECT_NAME} Core Qml Quick Svg Multimedia Network Sql)

if(ANDROID)
    set (QT_ANDROID_APP_NAME ${PROJECT_NAME})
    include(3rdparty/qt-android-cmake/AddQtAndroidApk.cmake)
    
    add_qt_android_apk(${PROJECT_NAME}_apk
                        ${PROJECT_NAME}
                        NAME "Story Teller"
                        PACKAGE_NAME "vitaliy.bondar.storyteller"
                        PACKAGE_SOURCES ${CMAKE_SOURCE_DIR}/android
                        BUILDTOOLS_REVISION "28.0.3"
                        DEPENDS ${ANDROID_EXTRA_LIBS}
)
else(ANDROID)
    add_subdirectory(tests)
endif()