add_library(cjson STATIC cJSON.c)

target_include_directories(cjson PUBLIC ${CMAKE_SOURCE_DIR}/include/cjson)
