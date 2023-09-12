#include <cctype>
#include <ios>
#include <iostream>
#include <sys/types.h>

#define RAWTERM_IMPLEMENTATION
#include "rawterm.h"

int main() {
    enable_raw_mode();
    enter_alt_screen();

    while (true) {
        Key k = process_keypress();
        if (k.code == 'q' && k.mod.empty()) break;

        std::string mods = "[";

        if (!k.mod.empty()) {
            for (size_t i = 0; i < k.mod.size(); ++i) {
                switch (k.mod[i]) {
                    case Mod::Control:
                        mods += " CTRL ";
                        break;
                    case Mod::Shift:
                        mods += " SHFT ";
                        break;
                    case Mod::Arrow:
                        mods += " ARROW ";
                        break;
                    case Mod::Escape:
                        mods += " ESC ";
                        break;
                    case Alt_L:
                        mods += " ALT_L ";
                    break;
                    case Function:
                        mods += " FUNCTION ";
                    break;
                }
            }
        }

        mods += "]";

        std::cout << "Key{ code: " << k.code << ", mods: " << mods << ", raw: " << k.raw << "}\r\n";
    }

    exit_alt_screen();

    return 0;
}
