#include <rawterm/color.h>
#include <rawterm/core.h>
#include <rawterm/cursor.h>

#include <iostream>

int main() {
    rawterm::enable_raw_mode();
    rawterm::enter_alt_screen();
    rawterm::Cursor c = rawterm::Cursor();
    c.reset();

    rawterm::Pos term_size = rawterm::get_term_size();
    bool toggle = false;

    for (unsigned int i = 1; i <= term_size.horizontal; i++) {
        for (unsigned int j = 1; j <= term_size.vertical; j++) {
            if (toggle) {
                std::cout << rawterm::set_background(" ", rawterm::Colors::black);
                toggle = false;
            } else {
                std::cout << rawterm::set_background(" ", rawterm::Colors::white);
                toggle = true;
            }
        }
    }

    auto k = rawterm::wait_for_input();
    rawterm::exit_alt_screen();
    return 0;
}
