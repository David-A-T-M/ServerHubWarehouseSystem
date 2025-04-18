file(GLOB_RECURSE SERVER_SOURCES *.cpp)

add_library(server STATIC ${SERVER_SOURCES})

target_include_directories(server PUBLIC ${CMAKE_SOURCE_DIR}/include/server)
target_include_directories(server PUBLIC ${CMAKE_SOURCE_DIR}/include/common)