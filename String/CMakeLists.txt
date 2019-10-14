cmake_minimum_required(VERSION 3.8)
project(my_string C)

set(CMAKE_CURRENT_SOURCE_DIR ${CMAKE_SOURCE_DIR}/src)
set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib)
set(SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/string.c ${CMAKE_CURRENT_SOURCE_DIR}/string.h)

add_library(my_str_t SHARED ${SOURCES})
#For testing purposes only!
enable_testing()

add_subdirectory(tests)
#add_executable(str_test ${SOURCES})