# Agregar biblioteca para el servidor
file(GLOB_RECURSE SERVER_SOURCES server/*.cpp)
add_library(server ${SERVER_SOURCES})

# Agregar biblioteca para common
#file(GLOB_RECURSE COMMON_SOURCES common/*.cpp)
#add_library(common ${COMMON_SOURCES})

# Vincular cJSON a ambas bibliotecas
target_include_directories(server PUBLIC ${CMAKE_SOURCE_DIR}/include)
#target_include_directories(common PUBLIC ${CMAKE_SOURCE_DIR}/include)
target_link_libraries(server cjson)
#target_link_libraries(common cjson)

# Agregar cJSON como biblioteca
add_library(cjson cjson/cJSON.c)
target_include_directories(cjson PUBLIC ${CMAKE_SOURCE_DIR}/include)