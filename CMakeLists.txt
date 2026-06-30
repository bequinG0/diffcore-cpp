cmake_minimum_required(VERSION 3.16)
project(diffcore LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Eigen3 REQUIRED)

# Статическая библиотека
add_library(diffcore STATIC
    src/conditions.cpp
    src/interpolate.cpp
)

target_include_directories(diffcore PUBLIC
    ${CMAKE_SOURCE_DIR}/include
    ${EIGEN3_INCLUDE_DIR}
)

target_link_libraries(diffcore PUBLIC Eigen3::Eigen)
