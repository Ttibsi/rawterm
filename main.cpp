#include <cctype>
#include <iostream>
#include <ios>
#include <sys/types.h>

#define RAWTERM_IMPLEMENTATION
#include "rawterm.h"

int main() {
    enable_raw_mode();
    enter_alt_screen();


    while (true) {
        Key k = process_keypress();
        if (k.code == 'q' && k.mod.empty()) break;

        std::cout << k.code;
        int len = k.mod.size();

        if (len) {
            std::cout << " (";
            
            while (len) {
                switch (k.mod[len]) {
                    case Mod::Control:
                        std::cout << "CTRL";
                        break;
                    case Mod::Shift:
                        std::cout << "SHFT";
                        break;
                }

                len--;
                if (len) std::cout << ", ";
            }

            std::cout << ")";
        }

        std::cout << "\r\n";
    }

    // while (std::cin.get(c) && c != 'q') {
    //     if (std::iscntrl(c)) {
    //         std::cout << static_cast<int>(c) << "\r\n";
    //     } else {
    //         std::cout << static_cast<int>(c) << " ('" << c << "')\r\n";
    //     }
    // }

    exitr_alt_screen();

    return 0;
}
