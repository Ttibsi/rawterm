#include "color.h"

#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>

#include "core.h"
#include "cursor.h"

namespace rawterm {
    Color::Color(const std::string& hex) {
        if (hex.at(0) != '#') {
            throw std::invalid_argument("Value passed not valid hex code");
        }

        if (hex.length() != 7) {
            throw std::invalid_argument("Hex code must be 7 characters long including the #");
        }

        std::istringstream ss(hex.substr(1, 2));
        ss >> std::hex >> red;

        ss.clear();
        ss.str(hex.substr(3, 2));
        ss >> std::hex >> green;

        ss.clear();
        ss.str(hex.substr(5, 2));
        ss >> std::hex >> blue;
    }

    const std::string Color::to_hex() {
        std::ostringstream ss;
        ss << "#" << std::uppercase << std::hex << std::setw(2) << std::setfill('0') << red
           << std::setw(2) << std::setfill('0') << green << std::setw(2) << std::setfill('0')
           << blue;

        return ss.str();
    }

    [[nodiscard]] std::string set_foreground(const std::string& s, const Color& color) {
        return "\x1B[38;2;" + std::to_string(color.red) + ';' + std::to_string(color.green) + ';' +
               std::to_string(color.blue) + 'm' + s + "\x1B[39m";
    }

    // Set the background colour based on a given Color object
    [[nodiscard]] std::string set_background(const std::string& s, const Color& color) {
        return "\x1B[48;2;" + std::to_string(color.red) + ';' + std::to_string(color.green) + ';' +
               std::to_string(color.blue) + 'm' + s + "\x1B[49m";
    }

    [[nodiscard]] std::string reset_foreground() {
        return "\x1B[39m";
    }

    [[nodiscard]] std::string reset_background() {
        return "\x1B[49m";
    }

    void set_terminal_background(const Color& col) {
        if (detail::is_debug()) {
            return;
        }

        rawterm::Cursor::save_cursor_position();
        rawterm::Pos term_size = rawterm::get_term_size();

        for (int v = 0; v < term_size.vertical; v++) {
            for (int h = 0; h < term_size.horizontal; h++) {
                std::cout << "\x1B[48;2;" << col << " "
                          << "\x1B[49m";
            }
        }

        rawterm::Cursor::load_cursor_position();
    }

}  // namespace rawterm
