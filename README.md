# rawterm
A c++ header-only library for working with raw mode in a terminal

See `main.cpp` for an example on how this works

Note that `escape.cpp` will print out the raw code that you press, and exists
for testing purposes only.

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
    char code;
    std::vector<Mod> mod;
    std::string raw;
};
```

* `code` - is the value you pressed (ie `a`)
* `mod` - is a vector of modifier keys
    * Values include: `Alt_L`, `Arrow`, `Control`, `Delete`, `Escape`, `Function`, `Shift`, `Space`
* `raw` - is the raw ascii string that the terminal takes in.
    * This mostly exists for debugging

The `get_term_size()` function provides the size of the terminal window as a `Pos` structure:

```cpp
struct Pos {
    int line;
    int col;
};
```

The following functions will accept a string and return your string wrapped in
the relevant escape codes for formatting:
- `bold()`
- `italic()`
- `underline()`
- `blink()`
- `inverse()`
- `hidden()`
- `strikethrough()`


You can also `clear_screen()`, or `save_cursor_position()` and
`load_cursor_position()`, or use the following functions to change the
appearance of your cursor:
- `cursor_block_blink()`
- `cursor_block()`
- `cursor_underscore_blink()`
- `cursor_underscore()`
- `cursor_pipe_blink()`
- `cursor_pipe()`

### Be Aware Of:
* `^m` and `enter` are the same key - they both pass `\x0d` code to the terminal
* `^i` and `tab` are the same key - they both pass `\x09` code to the terminal
* left alt and right alt are different keys - only left-alt has been handled
    * I think this is also the same for alt-gr on windows laptops
* `CMD` on macos doesn't return a unique ascii code, and is treated like the
base character being pressed
