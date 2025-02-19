# rawterm
A c++20 library for working with raw mode in a terminal, used for building
custom TUI applications without the old-school overhead of ncurses. 
Documentation is mostly provided in the `docs/` directory, where every header
file has a partner mardown file to describe it's methods usecases.

### How to use
As of `v4.0.0`, rawterm can be imported with cmake fetchcontent. Add the
following passage to your CMakeLists.txt file then link to the `rawterm`
library using `target_link_libraries`

```cmake
include(FetchContent)
fetchcontent_declare(
  rawterm
  GIT_REPOSITORY https://github.com/ttibsi/rawterm
  GIT_TAG        v4.0.7
)
fetchcontent_makeavailable(rawterm)
```

Alternatively, if you use `zig` as your build system, you can run

```console
zig fetch --save=rawterm git+https://github.com/ttibsi/rawterm
```

This has only been tested with zig 0.13.0

### Example
A minimal hello world example looks like this:

```cpp
#include <rawterm/color.h>
#include <rawterm/core.h>
#include <rawterm/cursor.h>
#include <rawterm/text.h>

#include <iostream>

int main() {
    // Raw mode automatically gets disabled at the end of the program
    rawterm::enable_raw_mode();

    rawterm::enter_alt_screen();

    // Create a cursor object to control the cursor, and reset it's position
    rawterm::Cursor c = rawterm::Cursor();
    c.reset();

    // Set the background colour to grey
    rawterm::set_terminal_background(rawterm::Colors::gray);

    // Display text to the screen. This can be done the same as any other
    // c++ text output. Rawterm provides a series of functions for customising
    // text appearance
    std::cout << rawterm::inverse("Hello world!\r\n");

    // Wait for user input
    auto k = rawterm::wait_for_input();

    return 0;
```

Please see the (examples)[https://github.com/Ttibsi/rawterm/tree/main/examples]
directory for more!

### New in version: 4.0.7
* Added unit testing in `tests/` directory
* Added zig build option
* `extras/menu.h` - New widget added for adding a vertical scrolling menu
* `extras/border.h` - New widget added for drawing borders around a given
vector of strings
* `screen.h` - Added hashing for `Pos` object to use as a key in
`std::unordered_map` and other hashed objects
* `text.h` - Add `raw_str` function to strip a string of any added ansi escape
codes
* Added new examples:
    * `examples/vertical_menu.cpp` for trialing menu.h
    * `examples/game.cpp` porting a simple "collect the coin" game to Rawterm
    * `examples/borders.cpp` for a simple display of how to use border.h

### Projects Using Rawterm
* [iris](https://github.com/ttibsi/iris) - Modal terminal text editor
* [re-view](https://github.com/ttibsi/re-view) - Regex viewer (uses the old header-only library)
* [gol-tui](https://github.com/ttibsi/gol-tui) - Conway's Game of Life (uses zig build system)

Feel free to add any projects written using rawterm to this list via PR!
