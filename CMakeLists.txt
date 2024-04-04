cmake_minimum_required(VERSION 3.22)
project(rawterm LANGUAGES CXX)

add_subdirectory(rawterm)

if(CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR)
    add_compile_options("-g")
    set(CMAKE_CXX_STANDARD 20)
    set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

    add_subdirectory(examples)
endif()
