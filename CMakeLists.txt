cmake_minimum_required(VERSION 3.0)
get_filename_component(
  PROJECT_NAME
  ${CMAKE_SOURCE_DIR}
  NAME_WE
)

message(${PROJECT_NAME})

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
include(Config.cmake)

add_subdirectory(src)