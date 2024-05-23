# rawterm
A c++20 library for working with raw mode in a terminal. See the
`examples/` directory for examples on how to use this library, and each
header file in `rawterm/` has a relevant markdown file in `docs/` when
relevant

See `examples/hello_world.cpp` for a simple place to start.

### How to use
As of `v4.0.0`, rawterm can be imported with cmake fetchcontent. Add the
following passage to your CMakeLists.txt file then link to the `rawterm`
library using `target_link_libraries`

```cmake
include(FetchContent)
fetchcontent_declare(
  rawterm
  GIT_REPOSITORY https://github.com/ttibsi/rawterm
  GIT_TAG        25989a41742bd4105a796c9607bb88260cd682fb
)
fetchcontent_makeavailable(rawterm)
```

### New in version: 4.0.0
* Refactor header-only library out into a cmake fetch-content project
* Added extra constructors and overloads to most classes
* Added support to disable any output for debugging purposes with
`RAWTERM_DEBUG` environment variable

* `core.h` - `process_keypress()` is now non-blocking, new `wait_for_input()`
function replaces blocking functionality
* `core.h` - Added `set_terminal_title()` function
* `core.h` - Added `partial_cmp()` function to `Pos` struct
* `color.h` - added function to set full terminal background
* `cursor.h` - Added `get_raw_location()` for raw terminal cursor location
* Added proper documentation

* Added new extras for functionality that wraps other rawterm sequences
    * Added `set_header()` function
    * Added `PaneManager` class for handling window splitting and manipulation

* Added new examples:
    * `red_blue_panes.cpp` for pane management
    * `cursor_position.cpp`

### Be Aware Of:
* `^m` and `enter` are the same key - they both pass `\x0d` code to the terminal
* `^i` and `tab` are the same key - they both pass `\x09` code to the terminal
* left alt and right alt are different keys - only left-alt has been handled
* `CMD` on macos doesn't return a unique ascii code, and is treated like the
base character being pressed

### Projects Using Rawterm
* [iris](https://github.com/ttibsi/iris) - Modal terminal text editor
* [re-view](https://github.com/ttibsi/re-view) - Regex viewer (uses the old header-only library)

Feel free to add any projects written using rawterm to this list via PR!
