cmake_minimum_required(VERSION 3.18)
project(Clippy LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CLIPPER2_UTILS OFF)
set(CLIPPER2_EXAMPLES OFF)
set(CLIPPER2_TESTS OFF)
set(CLIPPER2_USINGZ "OFF" CACHE STRING "Preempt cache default of USINGZ (only binding 2d)")
add_definitions(-D_HAS_EXCEPTIONS=0) # disable exceptions for STL

option(BUILD_SHARED_LIBS ON)
add_compile_options(-fPIC)

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../dist)

add_subdirectory(vendor/Clipper2/CPP)

file(GLOB_RECURSE SOURCE_FILES src/*.cpp)

add_library(Clippy SHARED ${SOURCE_FILES})

target_link_libraries(Clippy PRIVATE Clipper2)
target_include_directories(Clippy PUBLIC ${PROJECT_SOURCE_DIR}/include)
target_compile_features(Clippy PRIVATE cxx_std_17)
