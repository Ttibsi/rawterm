#include "color.h"

#include <format>
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

        red = static_cast<unsigned int>(std::stoi(hex.substr(1, 2), nullptr, 16));
        green = static_cast<unsigned int>(std::stoi(hex.substr(3, 2), nullptr, 16));
        blue = static_cast<unsigned int>(std::stoi(hex.substr(5, 2), nullptr, 16));
    }

    [[nodiscard]] const std::string Color::to_hex() const noexcept {
        std::string r_str = std::format("{:02X}", red);
        std::string g_str = std::format("{:02X}", green);
        std::string b_str = std::format("{:02X}", blue);

        return "#" + r_str + g_str + b_str;
    }

    [[nodiscard]] const std::string Color::to_str() const noexcept {
        return std::to_string(red) + ";" + std::to_string(green) + ";" + std::to_string(blue) + "m";
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
