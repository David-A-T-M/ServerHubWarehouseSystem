# Recopila todos los archivos fuente en la carpeta src/client
file(GLOB_RECURSE CLIENT_SOURCES *.c)

# Crea un ejecutable para el cliente
add_executable(client ${CLIENT_SOURCES})

# Incluye los directorios de encabezados necesarios
target_include_directories(client PUBLIC ${CMAKE_SOURCE_DIR}/include/client)
target_include_directories(client PUBLIC ${CMAKE_SOURCE_DIR}/include/common)
