cmake_minimum_required(VERSION 3.14)
project(HubWarehouseSystem C CXX)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 20)

# Ruta para includes
include_directories(${CMAKE_SOURCE_DIR}/include)

# Agregamos subdirectorios con su propio CMakeLists
add_subdirectory(src)
add_subdirectory(test)

# GoogleTest
add_subdirectory(external/googletest)
add_subdirectory(external/unity)
enable_testing()
