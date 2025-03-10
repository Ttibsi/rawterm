cmake_minimum_required(VERSION 3.22)
set(CMAKE_CXX_COMPILER "/usr/bin/clang++")
project(rawterm LANGUAGES CXX)

add_subdirectory(rawterm)

if(CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR)
    option(RUN_TESTS "Run unit tests" OFF)

    add_compile_options(-g)
    add_compile_options(-Wall)
    add_compile_options(-Wextra)
    add_compile_options(-pedantic)
    add_compile_options(-Wconversion)
    add_compile_options(-Wno-implicit-int-float-conversion)
    add_compile_options(-Wimplicit-fallthrough)

    set(CMAKE_CXX_STANDARD 20)
    set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

    add_subdirectory(examples)


    if(RUN_TESTS)
        enable_testing()
        message(STATUS "UNIT TESTS ENABLED")
        add_subdirectory(tests)
    endif()
endif()
