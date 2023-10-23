cmake_minimum_required(VERSION 3.18)

project(Clippy LANGUAGES CXX)

# CMake options
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

option(BUILD_SHARED_LIBS ON)

if(WIN32)
    add_compile_definitions(LIBCLIPPY_EXPORTS=__declspec(dllexport))
else()
    add_compile_definitions(LIBCLIPPY_EXPORTS=)
    add_compile_options(-fPIC)
endif()

# Clipper2 options
set(CLIPPER2_UTILS OFF)
set(CLIPPER2_EXAMPLES OFF)
set(CLIPPER2_TESTS OFF)
set(CLIPPER2_USINGZ "OFF" CACHE STRING "Preempt cache default of USINGZ (only binding 2d)")
add_definitions(-D_HAS_EXCEPTIONS=0) # disable exceptions for STL

add_subdirectory(Clipper2)

# Clippy headers
set(CLIPPY_HEADERS
    ${PROJECT_SOURCE_DIR}/include/clippy.h
)
source_group("Headers" FILES ${CLIPPY_HEADERS})

# Clippy sources
if(WIN32)
    set(CLIPPY_SOURCES
        ${PROJECT_SOURCE_DIR}/src/clippy.cpp
        ${PROJECT_SOURCE_DIR}/src/dllmain.cpp
    )
else()
    set(CLIPPY_SOURCES
        ${PROJECT_SOURCE_DIR}/src/clippy.cpp
    )
endif()
source_group("Sources" FILES ${CLIPPY_SOURCES})

set(ALL_FILES
    ${CLIPPY_HEADERS}
    ${CLIPPY_SOURCES}
)

if(WIN32)
    include(GenerateExportHeader)
endif()

add_library(Clippy SHARED ${ALL_FILES})

if(WIN32)
    generate_export_header(Clippy
        BASE_NAME Clippy
        EXPORT_MACRO_NAME Clippy_EXPORT
        EXPORT_FILE_NAME Clippy_Export.h
        STATIC_DEFINE Clippy_BUILT_AS_STATIC
    )
endif()

set_target_properties(Clippy
    PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/dist
    RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/dist
)

target_link_libraries(Clippy PRIVATE Clipper2)
target_include_directories(Clippy PUBLIC ${PROJECT_SOURCE_DIR}/include)
target_compile_features(Clippy PRIVATE cxx_std_17)
