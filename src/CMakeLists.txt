cmake_minimum_required(VERSION 3.0)

project(${PROJECT_NAME})

find_package(VTK REQUIRED)
include(${VTK_USE_FILE})

find_package(Boost REQUIRED)

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

target_include_directories(
  ${PROJECT_NAME}
  PRIVATE
  ${PROJECT_SOURCE_DIR}
  ${PROJECT_BINARY_DIR}
  ${Boost_INCLUDE_DIRS}
)