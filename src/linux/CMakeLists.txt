cmake_minimum_required (VERSION 2.8)
project (wiseUp)

FIND_PACKAGE (Threads)

# Appends the cmake/modules path to MAKE_MODULE_PATH variable.
set (CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake/modules ${CMAKE_MODULE_PATH})

include_directories (
  ${PROJECT_SOURCE_DIR}/include
)

add_subdirectory (src)
add_subdirectory (test)
