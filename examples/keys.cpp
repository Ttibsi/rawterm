#include <sys/types.h>

#include <cctype>
#include <deque>
#include <ios>
#include <iostream>

#include <rawterm/core.h>
#include <rawterm/color.h>
#include <rawterm/extras/extras.h>

// This is a demonstration on how `rawterm` works. You can compile this with
// `./build.sh` and run with `./out/keys`. Any keypress you enter then will
// show it's output as a Key object in the terminal. Press `q` to quit.

int main() {
    rawterm::enable_raw_mode();
    rawterm::enter_alt_screen();
    rawterm::enable_signals();

    rawterm::clear_screen();

    rawterm::Pos size = rawterm::get_term_size();
    std::string header = "Term size: " 
        + std::to_string(size.vertical) + ", " 
        + std::to_string(size.horizontal);

    std::cout << rawterm::set_header(header) << std::flush;

    while (true) {
        rawterm::Key k = rawterm::process_keypress();
        if (k == rawterm::Key('q', rawterm::Mod::None)) {
            break;
        }

        std::string mods = "[";
        while (!(k.mod.empty())) mods += " " + rawterm::to_string(k.getMod());
        mods += " ]";

        std::cout << "Key{ code: " << k.code << ", mods: " << mods
            << ", raw: " << k.raw << "}\r\n";
    }

    rawterm::exit_alt_screen();

    return 0;
}
