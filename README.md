# rawterm
A c++20 header-only library for working with raw mode in a terminal. See the
`examples/` directory for examples on how to use this library -- see
`examples/keys.cpp` for a simple place to start.


### How to use
All components are within the `rawterm` namespace.
Your startup and cleanup are handled with a few function calls:

```cpp
#include "rawterm.h"

int main() {
    rawterm::enable_raw_mode();
    rawterm::enter_alt_screen();

    // Do things here
    rawterm::exit_alt_screen();
    return 0;
}
```

We also provide the `Key` event to track and handle keypresses, returned from
the `process_keypress()` function, which takes no parameters. A `Key` looks
like this:

```cpp
struct Key {
    char code; // The key you pressed (ex: `a`)
    std::deque<Rawterm::Mod> mod; // A list of modifier keys you've pressed (ex `Rawterm::Mod::Shift`)
    std::string raw; // The raw ansi code pressed (mostly for debugging)
};
```
You can use the `getMod()` method to read the modifiers on a key, or use the 
`isCharInput()` method to check if the `kry.code` attribute is a printable
character.

The `get_term_size()` function provides the size of the terminal window as a
`Pos` structure that holds the `horizontal` and `vertical` cursor coordinates
as `std::size_t`


The following functions will accept a string and return your string wrapped in
the relevant escape codes for formatting:
- `rawterm::bold()`
- `rawterm::italic()`
- `rawterm::underline()`
- `rawterm::blink()`
- `rawterm::inverse()`
- `rawterm::hidden()`
- `rawterm::strikethrough()`

Changing the colour of text:
```cpp
rawterm::fg("Hello world in red!", {255, 0, 0})
```

Alternatively, you can pass in a pre-defined colour

You can also `clear_screen()`, or `save_cursor_position()` and
`load_cursor_position()`, or use the following functions to change the
appearance of your cursor:
- `cursor_block_blink()`
- `cursor_block()`
- `cursor_underscore_blink()`
- `cursor_underscore()`
- `cursor_pipe_blink()`
- `cursor_pipe()`
- `cursor_hide()`
- `cursor_show()`

Overload `inline void resize_handler(int);` to specify what happens on
terminal resize -- this handles the `SIGWINCH` signal

### Be Aware Of:
* `^m` and `enter` are the same key - they both pass `\x0d` code to the terminal
* `^i` and `tab` are the same key - they both pass `\x09` code to the terminal
* left alt and right alt are different keys - only left-alt has been handled
    * I think this is also the same for alt-gr on windows laptops
* `CMD` on macos doesn't return a unique ascii code, and is treated like the
base character being pressed
