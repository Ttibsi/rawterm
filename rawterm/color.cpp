#include "color.h"

namespace rawterm {
    std::string set_foreground(const std::string &s, const Color color) {
        return "\x1B[38;2;" + std::to_string(color.red) + ';' +
               std::to_string(color.green) + ';' + std::to_string(color.blue) +
               'm' + s + "\x1B[39m";
    }

    // Set the background colour based on a given Color object
    std::string set_background(const std::string &s, const Color color) {
        return "\x1B[48;2;" + std::to_string(color.red) + ';' +
               std::to_string(color.green) + ';' + std::to_string(color.blue) +
               'm' + s + "\x1B[49m";
    }

    std::string reset_foreground() { return "\x1B[39m"; }

    std::string reset_background() { return "\x1B[49m"; }
}
