#include <cctype>
#include <iostream>
#include <ios>
#include <sys/types.h>

#define RAWTERM_IMPLEMENTATION
#include "rawterm.h"

int main() {
    enable_raw_mode();

    char c;

    while (std::cin.get(c) && c != 'q') {
        if (std::iscntrl(c)) {
            std::cout << static_cast<int>(c) << "\r\n";
        } else {
            std::cout << static_cast<int>(c) << " ('" << c << "')\r\n";
        }
    }

    return 0;
}
