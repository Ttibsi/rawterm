#ifndef RAWTERM_COLOR_H
#define RAWTERM_COLOR_H

#include <cstdint>
#include <string>

namespace rawterm {
    struct Color {
        std::uint8_t red;
        std::uint8_t green;
        std::uint8_t blue;

        Color() : red(0), green(0), blue(0) {}
        Color(int x, int y, int z) : red(x), green(y), blue(z) {}
        Color(const std::string&);

        const std::string to_hex();

        friend std::ostream& operator<<(std::ostream& os, const Color& c) {
            return os << std::to_string(c.red) + ";" + std::to_string(c.green) + ";" +
                             std::to_string(c.blue) + "m";
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
    }  // namespace Colors

    [[nodiscard]] std::string set_foreground(const std::string&, const Color&);
    [[nodiscard]] std::string set_background(const std::string&, const Color&);
    [[nodiscard]] std::string reset_foreground();
    [[nodiscard]] std::string reset_background();

    void set_terminal_background(const Color&);
}  // namespace rawterm

#endif  // RAWTERM_COLOR_H
