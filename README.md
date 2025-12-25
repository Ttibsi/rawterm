# rawterm
A c++20 library for working with raw mode in a terminal, used for building
custom TUI applications without the old-school overhead of ncurses.
Documentation is mostly provided in the `docs/` directory, where every header
file has a partner mardown file to describe it's methods usecases.

### How to use
Add the following passage to your CMakeLists.txt file then link to the
`rawterm` library using `target_link_libraries`

```cmake
include(FetchContent)
fetchcontent_declare(
  rawterm
  GIT_REPOSITORY https://github.com/ttibsi/rawterm
  GIT_TAG        v4.0.8
)
fetchcontent_makeavailable(rawterm)
```

#### Example
A minimal hello world example looks like this:

```cpp
#include <rawterm/color.h>  // set_terminal_background
#include <rawterm/core.h>  // enable_raw_mode, enter_alt_screen, wait_for_input
#include <rawterm/cursor.h>  // Cursor
#include <rawterm/text.h>  // inverse

#include <iostream>

int main() {
    // Raw mode and alt_screen automatically gets disabled at the end of the program
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

### New in version: 4.0.8
* Add new Cursor::move overload to accept two integers
* Handle bug in border rendering crashing with no lines of text
* Removed zig build system
* borders now accept spans instead of enforcing vectors

### Projects Using Rawterm
* [iris](https://github.com/ttibsi/iris) - Modal terminal text editor
* [re-view](https://github.com/ttibsi/re-view) - Regex viewer (uses the old header-only library)
* [gol-tui](https://github.com/ttibsi/gol-tui) - Conway's Game of Life

Feel free to add any projects written using rawterm to this list via PR!
