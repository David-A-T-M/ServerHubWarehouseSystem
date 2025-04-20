file(GLOB_RECURSE SOURCES *.c)

add_executable(client_tests ${SOURCES} ${CMAKE_SOURCE_DIR}/src/client/client.c ${CMAKE_SOURCE_DIR}/external/unity/unity.c)
target_include_directories(client_tests PRIVATE ${CMAKE_SOURCE_DIR}/include ${CMAKE_SOURCE_DIR}/external/unity)
