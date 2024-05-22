#include <iostream>
#include <vector>

#include <rawterm/color.h>
#include <rawterm/core.h>
#include <rawterm/text.h>

int main() {
    std::vector<rawterm::Color> colors{
        rawterm::Colors::black, rawterm::Colors::gray, rawterm::Colors::silver,
        rawterm::Colors::white, rawterm::Colors::maroon, rawterm::Colors::red,
        rawterm::Colors::olive, rawterm::Colors::yellow, rawterm::Colors::green,
        rawterm::Colors::lime, rawterm::Colors::teal, rawterm::Colors::aqua,
        rawterm::Colors::navy, rawterm::Colors::blue, rawterm::Colors::purple,
        rawterm::Colors::fuchsia, rawterm::Colors::orange,
    };

    for (rawterm::Color color : colors) {
        std::cout << rawterm::set_foreground(rawterm::bold("Hello, world!\r\n"), color);
    }

    for (rawterm::Color color : colors) {
        std::cout << rawterm::set_background(rawterm::bold("Hello, world!"), color);
        std::cout << "\r\n";
    }
}
