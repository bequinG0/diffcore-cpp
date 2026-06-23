cmake_minimum_required(VERSION 3.16)
project(diffcore LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Eigen
find_package(Eigen3 REQUIRED)

# Исполняемый файл
add_executable(diffcore main.cpp)

# Исходники из src/
file(GLOB SRC_FILES src/*.cpp)

target_sources(diffcore PRIVATE ${SRC_FILES})

# Заголовки
target_include_directories(diffcore PRIVATE
    ${CMAKE_SOURCE_DIR}/include
    ${EIGEN3_INCLUDE_DIR}
)

# Линковка с Eigen
target_link_libraries(diffcore PRIVATE Eigen3::Eigen)

# Сборка в build/
set_target_properties(diffcore PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/build
)