#include <cctype>

#define RAWTERM_IMPLEMENTATION
#include "rawterm.h"

// This code runs with `./build.sh && ./build/escape`. It will print out the
// raw ascii code of any key you enter. It does not have a way out/to quit so
// you will need to kill your terminal window (or the executable with
// `pkill -9 escape` in another window).

int main() {
    enable_raw_mode();
    enter_alt_screen();

    while (true) {
        char seq[32];

        int ret = read(STDIN_FILENO, seq, sizeof(seq));
        if (ret < 0) {
            std::cerr
                << "ERROR: something went wrong during reading user input: "
                << std::strerror(errno) << std::endl;
            return 1;
        }

        std::string code;
        for (int i = 0; i < ret; ++i) {
            char buffer[5];
            std::snprintf(buffer, sizeof(buffer), "\\x%02x",
                          static_cast<unsigned char>(seq[i]));
            code += buffer;
        }

        std::cout << code << "\r\n";
    }

    exit_alt_screen();
    return 0;
}
