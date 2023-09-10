#include <cctype>
#include <iostream>
#include <ios>
#include <sys/types.h>

#define RAWTERM_IMPLEMENTATION
#include "rawterm.h"

int main() {
    enable_raw_mode();
    enter_alt_screen();

    char c;

    while (std::cin.get(c) && c != 'q') {
        if (std::iscntrl(c)) {
            std::cout << static_cast<int>(c) << "\r\n";
        } else {
            std::cout << static_cast<int>(c) << " ('" << c << "')\r\n";
        }
    }

    exitr_alt_screen();

    return 0;
}
