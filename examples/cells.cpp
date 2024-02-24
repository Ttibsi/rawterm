#include <iostream>
#include "../rawterm.h"

int main() {
    rawterm::enable_raw_mode();
    rawterm::enter_alt_screen();
    rawterm::move_cursor({1,1});

    rawterm::Pos term_size = rawterm::get_term_size();
    bool toggle = false;

    for (unsigned int i = 1; i <= term_size.horizontal; i++) {
        for (unsigned int j = 1; j <= term_size.vertical; j++) {
            if (toggle) {
                std::cout << rawterm::bg(" ", rawterm::black); 
                toggle = false;
            } else {
                std::cout << rawterm::bg(" ", rawterm::white); 
                toggle = true;
            }
        }
    }

    rawterm::process_keypress();
    rawterm::exit_alt_screen();
    return 0;
}

