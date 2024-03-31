#include <iostream>
#include <vector>

#include <rawterm/color.h>
#include <rawterm/core.h>
#include <rawterm/text.h>

int main() {
    std::vector<rawterm::Color> colors{
        rawterm::Colors::black, rawterm::Colors::gray, rawterm::Colors::white,
        rawterm::Colors::red, rawterm::Colors::orange, rawterm::Colors::yellow, 
        rawterm::Colors::lime, rawterm::Colors::green, rawterm::Colors::mint,
        rawterm::Colors::cyan, rawterm::Colors::azure, rawterm::Colors::blue,
        rawterm::Colors::violet, rawterm::Colors::purple, rawterm::Colors::magenta
    };

    for (rawterm::Color color : colors) {
        std::cout << rawterm::set_foreground(rawterm::bold("Hello, world!\r\n"), color);
    }

    for (rawterm::Color color : colors) {
        std::cout << rawterm::set_background(rawterm::bold("Hello, world!"), color);
        std::cout << "\r\n";
    }
}
