#include <sys/types.h>

#include <cctype>
#include <ios>
#include <iostream>

#include "../rawterm.h"

// This is a demonstration on how `rawterm` works. You can compile this with
// `./build.sh` and run with `./out/keys`. Any keypress you enter then will
// show it's output as a Key object in the terminal. Press `q` to quit.

int main() {
    rawterm::enable_raw_mode();
    rawterm::enter_alt_screen();
    rawterm::enable_signals();

    rawterm::clear_screen();
    rawterm::Pos size = rawterm::get_term_size();
    std::cout << "Term size: " << size.vertical << ", " << size.horizontal
        << "\r\n";

    while (true) {
        rawterm::Key k = rawterm::process_keypress();
        if (k.code == 'q' && k.mod.empty())
            break;

        std::string mods = "[";
        while (!(k.mod.empty())) mods += " " + rawterm::to_string(k.getMod());
        mods += " ]";

        std::cout << "Key{ code: " << k.code << ", mods: " << mods
            << ", raw: " << k.raw << "}\r\n";
    }

    rawterm::exit_alt_screen();

    return 0;
}
