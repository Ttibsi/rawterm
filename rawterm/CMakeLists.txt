set(CMAKE_CXX_STANDARD 20)

add_library(rawterm STATIC
    color.cpp
    core.cpp
    cursor.cpp
    text.cpp
)

target_include_directories(rawterm PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/..)
