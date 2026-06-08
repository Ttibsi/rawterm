#include <rawterm/color.h>
#include <rawterm/core.h>
#include <rawterm/extras/extras.h>

#include <cctype>
#include <chrono>
#include <iostream>
#include <thread>

// This is a demonstration on how `rawterm` works. Any keypress you enter
//  will show it's output as a Key object in the terminal. Press `q` to quit.

int main() {
    rawterm::enable_raw_mode();
    rawterm::enter_alt_screen();
    auto signals_thread = rawterm::enable_signals();

    rawterm::clear_screen();

    rawterm::Pos size = rawterm::get_term_size();
    std::string header =
        "Term size: " + std::to_string(size.vertical) + ", " + std::to_string(size.horizontal);

    std::cout << rawterm::set_header(header) << std::flush;

    // while (true) {
    auto k = rawterm::wait_for_input();
    // if (k == rawterm::Key('q')) {
    //     break;
    // } else {
    std::string mods = "[";
    while (!(k.mod.empty()))
        mods += " " + rawterm::to_string(k.getMod());
    mods += " ]";

    std::cout << "Key{ code: " << k.code << ", mods: " << mods << ", raw: " << k.raw << "}\r\n";

    using namespace std::this_thread;      // sleep_for, sleep_until
    using namespace std::chrono_literals;  // ns, us, ms, s, h, etc.
    using std::chrono::system_clock;

    sleep_for(10ns);
    sleep_until(system_clock::now() + 1s);
    // }
    // }

    // Optional explicit call to exit_alt_screen to return to standard screen.
    // This will happen even without calling this function explicitly
    rawterm::exit_alt_screen();

    // Don't forget to join back to the main thread
    signals_thread.join();
    return 0;
}
