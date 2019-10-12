cmake_minimum_required(VERSION 3.0)

project(${PROJECT_NAME})

find_package(VTK REQUIRED)
include(${VTK_USE_FILE})

file(
  GLOB
  PROJECT_SRC
  ${PROJECT_SOURCE_DIR}/*.c*
)

file(
  GLOB
  PROJECT_INC
  ${PROJECT_SOURCE_DIR}/*.h*
)

add_executable(
  ${PROJECT_NAME}
  ${PROJECT_SRC}
  ${PROJECT_INC}
)

target_link_libraries(
  ${PROJECT_NAME}
  PUBLIC
  ${VTK_LIBRARIES}
)