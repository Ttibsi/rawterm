#ifndef RAWTERM_BORDER_H
#define RAWTERM_BORDER_H

#include <rawterm/color.h>
#include <rawterm/cursor.h>
#include <rawterm/screen.h>

#include <optional>

namespace rawterm {
    struct Border {
        Region size;
        int border_padding = 0;

        std::optional<char> border_char;
        std::optional<std::string> border_title;
        std::optional<Color> border_color;

        Border(Region size) : size(size), border_char(std::nullopt) {}
        Border(Region size, char border_char) : size(size), border_char(border_char) {}

        Border& set_padding(int padding) {
            border_padding = padding;
            return *this;
        }

        Border& set_title(const std::string& title) {
            border_title = title;
            return *this;
        }

        Border& set_color(const Color& color) {
            border_color = color;
            return *this;
        }

        [[nodiscard]] const std::string render() const {
            std::string render = "";
            return render;
        };

        void draw(Cursor& cur) const {
            // Disable if rawterm_debug

            // Check if terminal size is reasonable
            // abort drawing if it won't fit on screen
            // Truncate title if it's shorter than the width
            // Move cursor to top left and start drawing
        }
    };

}  // namespace rawterm

#endif  // RAWTERM_BORDER_H
