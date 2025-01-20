include_directories(${PROJECT_SOURCE_DIR}/rawterm)
add_compile_options(-g)

add_executable(test_exe
    main.cpp # DO not modify

    border_test.cpp
    color_test.cpp
    core_test.cpp
    cursor_test.cpp
    menu_test.cpp
    screen_test.cpp
    text_test.cpp
)

target_link_libraries(test_exe PUBLIC rawterm)
