# Incluir Unity directamente
set(UNITY_SOURCES external/unity/unity.c)
set(UNITY_HEADERS external/unity/unity.h external/unity/unity_internals.h)

# Asegúrate de que los tests incluyan Unity
include_directories(${CMAKE_SOURCE_DIR}/external/unity)
