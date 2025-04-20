# Agregar biblioteca para el servidor
file(GLOB_RECURSE SERVER_SOURCES server/*.cpp)
add_library(server ${SERVER_SOURCES})

# Agregar ejecutable para el cliente
file(GLOB_RECURSE CLIENT_SOURCES client/*.c)
add_executable(client ${CLIENT_SOURCES})

# Agregar cJSON como biblioteca
add_library(cjson cjson/cJSON.c)
target_include_directories(cjson PUBLIC ${CMAKE_SOURCE_DIR}/include)

# Configurar directorios de encabezados
target_include_directories(server PUBLIC ${CMAKE_SOURCE_DIR}/include/server)
target_include_directories(server PUBLIC ${CMAKE_SOURCE_DIR}/include/common)

target_include_directories(client PUBLIC ${CMAKE_SOURCE_DIR}/include/client)
target_include_directories(client PUBLIC ${CMAKE_SOURCE_DIR}/include/common)

# Vincular cJSON a ambas bibliotecas
target_link_libraries(server cjson)
target_link_libraries(client cjson)
