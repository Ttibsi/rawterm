#ifndef RAWTERM_COLOR_H
#define RAWTERM_COLOR_H

#include <string>

namespace rawterm {
    struct Color {
        unsigned int red;
        unsigned int green;
        unsigned int blue;

        Color() : red(0u), green(0u), blue(0u) {}
        Color(unsigned int x, unsigned int y, unsigned int z) : red(x), green(y), blue(z) {}
        Color(const std::string&);

        [[nodiscard]] const std::string to_hex() const noexcept;
        [[nodiscard]] const std::string to_str() const noexcept;

        friend std::ostream& operator<<(std::ostream& os, const Color& c) {
            return os << std::to_string(c.red) + ";" + std::to_string(c.green) + ";" +
                             std::to_string(c.blue) + "m";
        }

        [[nodiscard]] bool operator==(const Color& other) const {
            return this->red == other.red && this->blue == other.blue && this->green == other.green;
        }
    };

    // Color presets
    namespace Colors {
        inline const Color black {0, 0, 0};
        inline const Color gray {128, 128, 128};
        inline const Color silver {192, 192, 192};
        inline const Color white {255, 255, 255};
        inline const Color maroon {128, 0, 0};
        inline const Color red {255, 0, 0};
        inline const Color olive {128, 128, 0};
        inline const Color yellow {255, 255, 0};
        inline const Color green {0, 128, 0};
        inline const Color lime {0, 255, 0};
        inline const Color teal {0, 128, 128};
        inline const Color aqua {0, 255, 255};
        inline const Color navy {0, 0, 128};
        inline const Color blue {0, 0, 255};
        inline const Color purple {128, 0, 128};
        inline const Color fuchsia {255, 0, 255};
        inline const Color orange {255, 127, 0};
        inline const std::string reset = "\x1b[0m";
    }  // namespace Colors

    [[nodiscard]] std::string set_foreground(const std::string&, const Color&);
    [[nodiscard]] std::string set_background(const std::string&, const Color&);
    [[nodiscard]] std::string reset_foreground();
    [[nodiscard]] std::string reset_background();

    void set_terminal_background(const Color&);
}  // namespace rawterm

#endif  // RAWTERM_COLOR_H
