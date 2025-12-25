### main/HEAD

### v4.0.8 - 25/Dec/2025
* Add new Cursor::move overload to accept two integers
* Handle bug in border rendering crashing with no lines of text
* Removed zig build system
* borders now accept spans instead of enforcing vectors

### v4.0.7
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

### v4.0.6
* `extras/pane.h` - Added handling for blacklisting Regions from the cursor
* Fixed a bug with cursor move function (vorlac - #50)

### v4.0.5
* Split out the `Pos` struct into `screen.h`
* Added a `Region` struct for defining a square between two points on the
screen

### v4.0.4
* `extras/pane.h` - Tweaked location for content to constructors
* `core.h` - Added `Operator-=` overload to `Pos` class for other Pos objects
* Resolved miscellaneous compiler warnings

### v4.0.3
* Resolved missed compiler warnings
* `extras/pane.h` - Updated `draw()` function to check for char type instead of
assuming strings on each line

### v4.0.2
* `core.h` - Updated `process_keypress()` to return an `std::optional`
* `core.h` - `process_keypress()` now throws exceptions instead of relying on
c-style `perror` error handling
* `core.h` - Updated `Key` with new char-only constructor

### v4.0.1
* Resolved missed compiler warnings

### v4.0.0
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

### v3.0.0
* Windows support

### v2.4.1
* Added tab key to modifiers list
* Added wstring handling
* Added functions for relative cursor movement
* Converted `getMod()` and `isCharInput()` to methods on `Key`
* Added operator== to Pos struct

### v2.4.0
* Reverted failed SIGWINCH handling
* Added handling for terminal job system (SIGSTP/SIGCONT)
* Added new functions for custom handlers for the user

### v2.3.0
* Added handling for SIGWINCH (Sig win change) for terminal resizing
* Fixed incorrect "closing" ansi code for colour highlighting

### v2.2.5
* Fixed incorrect ansi code for entering/leaving raw mode

### v2.2.4
* Fixed cursor changes not flushing the escape code
* Added `rawterm::to_string` to get string values of the `rawterm::Mod` enum

### v2.2.3
* Updated `offset_cursor` to used signed ints instead of a `rawterm::Pos`

### v2.2.2
* Fixed incorrect naming of `Pos` members

### v2.2.1
* Inlined some functions to prevent "duplicate symbol" linker issue

### v2.2.0
* `key.mod` is now a `std::deque` instead of an std::vector
* Added `rawterm::Mod::None` as a default value
* Added `getMod()` to read modifiers sequentially
* Fixed error with `alt_l + capital letter` as it was using multiple modifiers
    * `Alt_L` and `Shift`

### v2.1.0
* Added `isCharInput()` function
* ^m now returns `Enter` modifier value instead of `Control`
* `Pos` struct members renamed to be clearer

### v2.0.0
* Added descriptive comments to each function
* Added `Pos` structure, now returned by `get_term_size` and accepted by
`move_cursor` (eczbek - #10)
* Made `asciiLetters` into an unordered set for better accessing (eczbek - #10)
* Replaced if-else chains with switches (eczbek - #10)
* Wrapped everything in `rawterm` namespace (eczbek - #10)

### v1.7.1
* Remove unneccesary copying (@Eczbek - #9)

### v1.7.0
* Fixed occasional segfault with reading `esc` char

### v1.6.0
* Added the backspace key detection

### v1.5.0
*  `get_term_size()` now returns a pair of std::size_t instead of ints
* `Mod` is now an `enum class` instead of an `enum`
* Added cursor appearance functions

### v1.4.0
* save and load cursor position (SCO sequences)

### v1.3.1
* Exported formatting functions ("header" section of header)

### v1.3.0
* Added text formatting functions

### v1.2.1
* Fix error with move_cursor() not actually updating

### v1.2.0
* Added  `move_cursor()`
* Added  `clear_screen()`

### v1.1.0
* Added `get_term_size()`
