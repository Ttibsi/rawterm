include_directories(${PROJECT_SOURCE_DIR}/rawterm)

add_executable(border borders.cpp)
add_executable(cells cells.cpp)
add_executable(colors colors.cpp)
add_executable(cursor_position cursor_position.cpp)
add_executable(game game.cpp)
add_executable(hello_world hello_world.cpp)
add_executable(keys keys.cpp)
add_executable(raw_escapes raw_escapes.cpp)
add_executable(red_blue_panes red_blue_panes.cpp)
add_executable(vertical_menu vertical_menu.cpp)

target_link_libraries(border PUBLIC rawterm)
target_link_libraries(cells PUBLIC rawterm)
target_link_libraries(colors PUBLIC rawterm)
target_link_libraries(cursor_position PUBLIC rawterm)
target_link_libraries(game PUBLIC rawterm)
target_link_libraries(hello_world PUBLIC rawterm)
target_link_libraries(keys PUBLIC rawterm)
target_link_libraries(raw_escapes PUBLIC rawterm)
target_link_libraries(red_blue_panes PUBLIC rawterm)
target_link_libraries(vertical_menu PUBLIC rawterm)
