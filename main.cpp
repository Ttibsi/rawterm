#include <sys/types.h>

#include <cctype>
#include <ios>
#include <iostream>

#include "rawterm.h"

// This is a demonstration on how `rawterm` works. You can compile this with
// `./build.sh` and run with `./build/main`. Any keypress you enter then will
// show it's output as a Key object in the terminal. Press `q` to quit.

int main() {
    rawterm::enable_raw_mode();
    rawterm::enter_alt_screen();
    rawterm::Pos size = rawterm::get_term_size();
    std::cout << "Term size: " << size.line << ", " << size.col << "\r\n";

    int count = 0;

    while (true) {
        rawterm::Key k = rawterm::process_keypress();
        if (k.code == 'q' && k.mod.empty())
            break;

        count++;
        if (count == 5) {
            rawterm::clear_screen();
            rawterm::move_cursor({0,0});
            count = 0;
        }

        std::string mods = "[";

        if (!k.mod.empty()) {
            for (size_t i = 0; i < k.mod.size(); ++i) {
                switch (k.mod[i]) {
                case rawterm::Mod::Alt_L:
                    mods += " ALT_L ";
                    break;
                case rawterm::Mod::Arrow:
                    mods += " ARROW ";
                    break;
                case rawterm::Mod::Backspace:
                    mods += " Backspace ";
                    break;
                case rawterm::Mod::Control:
                    mods += " CTRL ";
                    break;
                case rawterm::Mod::Delete:
                    mods += " DEL ";
                    break;
                case rawterm::Mod::Escape:
                    mods += " ESC ";
                    break;
                case rawterm::Mod::Function:
                    mods += " FUNCTION ";
                    break;
                case rawterm::Mod::Shift:
                    mods += " SHFT ";
                    break;
                case rawterm::Mod::Space:
                    mods += " SPACE ";
                    break;
                default:
                    mods += " OTHER ";
                    break;
                }
            }
        }

        mods += "]";

        std::cout << "Key{ code: " << k.code << ", mods: " << mods
                  << ", raw: " << k.raw << "}\r\n";
    }

    rawterm::exit_alt_screen();

    return 0;
}
