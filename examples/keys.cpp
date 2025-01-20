#include <rawterm/color.h>
#include <rawterm/core.h>
#include <rawterm/extras/extras.h>

#include <cctype>
#include <iostream>

// This is a demonstration on how `rawterm` works. Any keypress you enter
//  will show it's output as a Key object in the terminal. Press `q` to quit.

int main() {
    rawterm::enable_raw_mode();
    rawterm::enter_alt_screen();
    rawterm::enable_signals();

    rawterm::clear_screen();

    rawterm::Pos size = rawterm::get_term_size();
    std::string header =
        "Term size: " + std::to_string(size.vertical) + ", " + std::to_string(size.horizontal);

    std::cout << rawterm::set_header(header) << std::flush;

    while (true) {
        auto k = rawterm::process_keypress();
        if (k.has_value()) {
            if (k.value() == rawterm::Key('q')) {
                break;
            } else {
                auto key_value = k.value();
                std::string mods = "[";
                while (!(key_value.mod.empty()))
                    mods += " " + rawterm::to_string(key_value.getMod());
                mods += " ]";

                std::cout << "Key{ code: " << key_value.code << ", mods: " << mods
                          << ", raw: " << key_value.raw << "}\r\n";
            }
        }
    }

    // Optional explicit call to exit_alt_screen to return to standard screen.
    // This will happen even without calling this function explicitly
    rawterm::exit_alt_screen();
    return 0;
}
