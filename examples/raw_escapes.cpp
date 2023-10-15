#include <cctype>
#include <string>
#include <sstream>

#include "../rawterm.h"

// This code runs with `./build.sh && ./build/escape`. It will print out the
// raw ascii code of any key you enter. It does not have a way out/to quit so
// you will need to kill your terminal window (or the executable with
// `pkill -9 escape` in another window).

int main() {
    rawterm::enable_raw_mode();
    rawterm::enter_alt_screen();

    while (true) {
        // char seq[32];
        std::string seq;

        int ret = read(STDIN_FILENO, seq.data(), 32);
        if (ret < 0) {
            std::cerr
                << "ERROR: something went wrong during reading user input: "
                << std::strerror(errno) << std::endl;
            break;
        }

        std::string code;
        for (int i = 0; i < ret; ++i) {
            // char buffer[5];
            // std::snprintf(buffer, sizeof(buffer), "\\x%02x",
            //               static_cast<unsigned char>(seq[i]));
            // code += buffer;

            std::stringstream ss;
            ss << std::hex << "\\x" << static_cast<unsigned int>(seq[i]);
            code += ss.str();
        }

        std::cout << code << "\r\n";
    }

    rawterm::exit_alt_screen();
    return 0;
}
