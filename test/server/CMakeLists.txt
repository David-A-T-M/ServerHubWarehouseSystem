file(GLOB_RECURSE SERVER_TESTS *.cpp)

add_executable(server_tests ${SERVER_TESTS})

target_link_libraries(server_tests
        server
        cjson
        gtest_main
)

include(GoogleTest)
gtest_discover_tests(server_tests)
