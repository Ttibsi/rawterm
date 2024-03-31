#ifndef RAWTERM_COLOR_H
#define RAWTERM_COLOR_H

#include <cstdint>
#include <string>

namespace rawterm {
    struct Color {
        std::uint8_t red;
        std::uint8_t green;
        std::uint8_t blue;
    };

    // Color presets
    namespace Colors {
        inline const Color black{ 0, 0, 0 };
        inline const Color gray{ 127, 127, 127 };
        inline const Color white{ 255, 255, 255 };
        inline const Color red{ 255, 0, 0 };
        inline const Color orange{ 255, 127, 0 };
        inline const Color yellow{ 255, 255, 0 };
        inline const Color lime{ 127, 255, 0 };
        inline const Color green{ 0, 255, 0 };
        inline const Color mint{ 0, 255, 127 };
        inline const Color cyan{ 0, 255, 255 };
        inline const Color azure{ 0, 127, 255 };
        inline const Color blue{ 0, 0, 255 };
        inline const Color violet{ 127, 0, 255 };
        inline const Color purple{ 255, 0, 255 };
        inline const Color magenta{ 255, 0, 127 };
    } 

    std::string set_foreground(const std::string&, const Color);
    std::string set_background(const std::string&, const Color);
    std::string reset_foreground();
    std::string reset_background();
}

#endif // RAWTERM_COLOR_H
