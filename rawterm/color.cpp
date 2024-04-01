#include <iostream>
#include <string>

#include "core.h"
#include "color.h"
#include "cursor.h"

namespace rawterm {
    [[nodiscard]] std::string set_foreground(const std::string &s, const Color color) {
        return "\x1B[38;2;" + std::to_string(color.red) + ';' +
               std::to_string(color.green) + ';' + std::to_string(color.blue) +
               'm' + s + "\x1B[39m";
    }

    // Set the background colour based on a given Color object
    [[nodiscard]] std::string set_background(const std::string &s, const Color color) {
        return "\x1B[48;2;" + std::to_string(color.red) + ';' +
               std::to_string(color.green) + ';' + std::to_string(color.blue) +
               'm' + s + "\x1B[49m";
    }

    [[nodiscard]] std::string reset_foreground() { return "\x1B[39m"; }

    [[nodiscard]] std::string reset_background() { return "\x1B[49m"; }

    void set_terminal_background(const Color& col) {
        rawterm::Cursor::save_cursor_position();

        rawterm::Pos term_size = rawterm::get_term_size();
        int len = term_size.vertical * term_size.horizontal;
        std::string background = "\x1B[48;2;" + std::to_string(col.red) + ';' +
            std::to_string(col.green) + ';' + std::to_string(col.blue) + 'm';

        std::cout << background << std::string(len, ' ');
        rawterm::Cursor::load_cursor_position();
    }
}
