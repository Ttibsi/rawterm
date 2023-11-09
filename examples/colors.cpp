#include "../rawterm.h"
#include <iostream>
#include <vector>

int main() {
  std::vector<rawterm::Color> colors{
      rawterm::black,  rawterm::gray,   rawterm::white,  rawterm::red,
      rawterm::orange, rawterm::yellow, rawterm::lime,   rawterm::green,
      rawterm::mint,   rawterm::cyan,   rawterm::azure,  rawterm::blue,
      rawterm::violet, rawterm::purple, rawterm::magenta};

  for (rawterm::Color color : colors) {
    std::cout << rawterm::fg(rawterm::bold("Hello, world!\r\n"), color);
  }

  for (rawterm::Color color : colors) {
    std::cout << rawterm::bg(rawterm::bold("Hello, world!\r\n"), color);
  }
}
