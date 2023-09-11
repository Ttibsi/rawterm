#include <cctype>
#include <iostream>
#include <ios>
#include <sys/types.h>

#define RAWTERM_IMPLEMENTATION
#include "rawterm.h"

int main() {
    enable_raw_mode();
    enter_alt_screen();

    while (true) process_keypress();
    // while (true) {
    //     Key k = process_keypress();
    //     if (k.code == 'q' && k.mod.empty()) break;
    //
    //     std::cout << k.code;
    //
    //     if (!k.mod.empty()) {
    //         std::cout << " (";
    //
    //         for (size_t i = 0; i < k.mod.size(); ++i) {
    //             switch (k.mod[i]) {
    //                 case Mod::Control:
    //                     std::cout << "CTRL";
    //                     break;
    //                 case Mod::Shift:
    //                     std::cout << "SHFT";
    //                     break;
    //                 case Mod::Arrow:
    //                     std::cout << "ARROW";
    //                     break;
    //                 case Mod::Escape:
    //                     std::cout << "ESC";
    //                     break;
    //             }
    //
    //             if (i < k.mod.size() - 1) {
    //                 std::cout << ", ";
    //             }
    //         }
    //
    //         std::cout << ")";
    //     }
    //
    //     std::cout << " (" << k.raw << ")";
    //     std::cout << "\r\n";
    // }

    exit_alt_screen();

    return 0;
}
