#include <rawterm/core.h>
#include <rawterm/cursor.h>
#include <rawterm/extras/menu.h>
#include <rawterm/screen.h>

#include <iostream>
#include <string>
#include <vector>

int main() {
    // Create values to construct the menu
    auto menu_region = rawterm::Region({3, 3}, {8, 8});
    std::vector<std::string> options = {"Hello world", "Foo", "Bar", "This is some text"};

    // Set up the menu
    auto vert_menu = rawterm::VMenu(menu_region, options);
    vert_menu.border = true;

    // Clear screen and reset cursor
    rawterm::enable_raw_mode();
    rawterm::clear_screen();
    rawterm::Cursor cur = rawterm::Cursor(menu_region.top_left);
    cur.reset();

    // helper lambda to show selected element
    auto display_selected = [&cur, &vert_menu](std::string_view selected) {
        // Place cursor for display
        cur.move(vert_menu.dims.bottom_right);
        cur.move_left(vert_menu.dims.bottom_right.horizontal);

        std::cout << "Selected: " << selected;

        // Reset cursor pos
        cur.move(
            {vert_menu.dims.top_left.vertical + vert_menu.active_opt - 1,
             vert_menu.dims.top_left.horizontal});
    };

    // Set up user input and render loop
    bool redraw = true;
    while (true) {
        if (redraw) {
            cur.move(vert_menu.dims.top_left);
            rawterm::clear_screen();
            std::cout << vert_menu.render();
        }
        redraw = false;

        auto k = rawterm::process_keypress();
        if (k.has_value()) {
            // Press q to quit
            if (k.value() == rawterm::Key('q')) {
                break;

                // Press j to select the next option
            } else if (k.value() == rawterm::Key('j')) {
                vert_menu.next();
                redraw = true;

                // Press k to select the previous option
            } else if (k.value() == rawterm::Key('k')) {
                vert_menu.prev();
                redraw = true;

                // Press enter to select an option
            } else if (k.value() == rawterm::Key('m', rawterm::Mod::Enter)) {
                display_selected(vert_menu.get_current_opt());
            }
        }
    }

    return 0;
}
