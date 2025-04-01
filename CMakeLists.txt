cmake_minimum_required(VERSION 3.10)

project(osg2gltf LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(osg2gltf
    ${PROJECT_SOURCE_DIR}/include/osg_types/Data_variance.hpp
    ${PROJECT_SOURCE_DIR}/include/osg_types/Geode.hpp
    ${PROJECT_SOURCE_DIR}/include/osg_types/Geometry.hpp
    ${PROJECT_SOURCE_DIR}/include/osg_types/Group.hpp
    ${PROJECT_SOURCE_DIR}/include/osg_types/Material.hpp
    ${PROJECT_SOURCE_DIR}/include/osg_types/Matrix_transform.hpp
    ${PROJECT_SOURCE_DIR}/include/osg_types/Node.hpp
    ${PROJECT_SOURCE_DIR}/include/osg_types/State_set.hpp
    ${PROJECT_SOURCE_DIR}/include/osg_types/Texture2d.hpp
    ${PROJECT_SOURCE_DIR}/include/Application.hpp
    ${PROJECT_SOURCE_DIR}/src/Application.cpp
    ${PROJECT_SOURCE_DIR}/src/main.cpp
)

target_include_directories(osg2gltf PRIVATE
    ${PROJECT_SOURCE_DIR}/include
)
