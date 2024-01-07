#include <sys/types.h>

#include <cctype>
#include <ios>
#include <iostream>

#include "../rawterm.h"

// This is a demonstration on how `rawterm` works. You can compile this with
// `./build.sh` and run with `./build/main`. Any keypress you enter then will
// show it's output as a Key object in the terminal. Press `q` to quit.

int main() {
  rawterm::enable_raw_mode();
  rawterm::enter_alt_screen();
  rawterm::enable_signals();

  rawterm::clear_screen();
  rawterm::Pos size = rawterm::get_term_size();
  std::cout << "Term size: " << size.vertical << ", " << size.horizontal
            << "\r\n";

  while (true) {
    rawterm::Key k = rawterm::process_keypress();
    if (k.code == 'q' && k.mod.empty())
      break;

    std::string mods = "[";

    while (!(k.mod.empty())) {
      switch (rawterm::getMod(&k)) {
      case rawterm::Mod::Alt_L:
        mods += " Alt_L ";
        break;
      case rawterm::Mod::Arrow:
        mods += " Arrow ";
        break;
      case rawterm::Mod::Backspace:
        mods += " Backspace ";
        break;
      case rawterm::Mod::Control:
        mods += " Control ";
        break;
      case rawterm::Mod::Delete:
        mods += " Delete ";
        break;
      case rawterm::Mod::Enter:
        mods += " Enter ";
        break;
      case rawterm::Mod::Escape:
        mods += " Escape ";
        break;
      case rawterm::Mod::Function:
        mods += " Function ";
        break;
      case rawterm::Mod::None:
        mods += " None ";
        break;
      case rawterm::Mod::Shift:
        mods += " Shift ";
        break;
      case rawterm::Mod::Space:
        mods += " Space ";
        break;
      case rawterm::Mod::Unknown:
        mods += " Unknown ";
        break;
      }
    }

    mods += "]";

    std::cout << "Key{ code: " << k.code << ", mods: " << mods
              << ", raw: " << k.raw << "}\r\n";
  }

  rawterm::exit_alt_screen();

  return 0;
}
