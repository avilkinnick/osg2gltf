cmake_minimum_required(VERSION 3.10)

project(osg2gltf LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(osg2gltf
    ${PROJECT_SOURCE_DIR}/include/Application.hpp
    ${PROJECT_SOURCE_DIR}/src/Application.cpp
    ${PROJECT_SOURCE_DIR}/src/main.cpp
)

target_include_directories(osg2gltf PRIVATE
    ${PROJECT_SOURCE_DIR}/include
)
