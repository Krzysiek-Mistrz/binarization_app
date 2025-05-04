cmake_minimum_required(VERSION 3.10)
project(binarization_app VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(Qt5 REQUIRED COMPONENTS Widgets Concurrent)
find_package(OpenCV REQUIRED)

qt5_add_resources(RESOURCES_RCC resources/resources.qrc)

set(SOURCES
    src/main.cpp
    src/mainwindow.cpp
    ${RESOURCES_RCC}
)

set(HEADERS
    include/mainwindow.hpp
)

add_executable(${PROJECT_NAME}
    ${SOURCES}
    ${HEADERS}
)

target_include_directories(${PROJECT_NAME} PRIVATE
    ${OpenCV_INCLUDE_DIRS}
    ${CMAKE_SOURCE_DIR}/include
)

target_link_libraries(${PROJECT_NAME}
    PRIVATE
        Qt5::Widgets
        Qt5::Concurrent
        ${OpenCV_LIBS}
)
