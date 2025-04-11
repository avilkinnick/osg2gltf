cmake_minimum_required(VERSION 3.10)

project(osg2gltf LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(osg2gltf
    ${PROJECT_SOURCE_DIR}/include/osg/Geode.hpp
    ${PROJECT_SOURCE_DIR}/include/osg/Geometry.hpp
    ${PROJECT_SOURCE_DIR}/include/osg/Group.hpp
    ${PROJECT_SOURCE_DIR}/include/osg/Image.hpp
    ${PROJECT_SOURCE_DIR}/include/osg/Matrix_transform.hpp
    ${PROJECT_SOURCE_DIR}/include/osg/Object.hpp
    ${PROJECT_SOURCE_DIR}/include/osg/State_set.hpp
    ${PROJECT_SOURCE_DIR}/include/osg/Texture2d.hpp
    ${PROJECT_SOURCE_DIR}/include/Application.hpp
    ${PROJECT_SOURCE_DIR}/src/Application.cpp
    ${PROJECT_SOURCE_DIR}/src/main.cpp
)

target_include_directories(osg2gltf PRIVATE
    ${PROJECT_SOURCE_DIR}/include
)
