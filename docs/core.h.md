# core.h
The core of rawterm, and the parts that haven't been split off from the original
header-only library.

### Structs

#### Key
A representation of a key pressed, including held modifier keys. 

* `[[nodiscard]] rawterm::Mod getMod();`
Get the first modifier key using a deque pop

* `[[nodiscard]] bool isCharInput();`
Check if the key has a displayable character

Example usage:

```cpp
// assuming rawterm/core.h and rawterm.cursor.h is included 
auto cur = rawterm::Cursor();

while (true) {
    rawterm::Key k = rawterm::process_keypress();
    rawterm::Mod modifier = rawterm::getMod(&k);

    if (k.code == 'h' && modifier == rawterm::Mod::None) {
        cur.move_left();
    } else if (k.code == 'j' && modifier == rawterm::Mod::None) {
        cur.move_down();
    } else if (k.code == 'k' && modifier == rawterm::Mod::None) {
        cur.move_up();
    } else if (k.code == 'l' && modifier == rawterm::Mod::None) {
        cur.move_right();
    }
}

```

#### Pos
* A representation of a position on the screen. 
* Terminal positions start at 1,1
* A parent class of a Cursor, and used to pass terminal positions around to 
other functions/methods. Feel free to use this struct to represent positions
in your own code.

### functions

* `void disable_raw_mode();`
* `[[maybe_unused]] const int enable_raw_mode();`
Toggle between raw content entry mode for the terminal. Should work on both
windows and unix systems. This is required for the rawterm to process control 
keys and other characters. Calling `disable_raw_mode()` is not required as
rawterm automatically resets on exit.

Further reading: `https://en.wikipedia.org/wiki/Terminal_mode`

* `void enter_alt_screen();`
* `void exit_alt_screen();`
Terminals have a second buffer screen that most TUI applications make use of
to draw their interfaces in. 

* `void enable_signals();`
* `void sigtstp_handler(std::function<void(void)>);`
* `void sigcont_handler(std::function<void(void)>);`
On unix-based systems, signals are used to handle functionality such as 
backgrounding and ctrl+c to kill the app. Rawterm requires explicit 
enabling of ctrl-signal handling. You can overload the functions that trigger
on specific signals. The default functionality is as expected. Calling this
function on windows does nothing.

* `[[nodiscard]] rawterm::Key process_keypress();`
Return a Key object based on what the user has entered. Often used within a 
while loop to get continual input. Currently, this function blocks the main
control flow

* `[[nodiscard]] rawterm::Pos get_term_size();`
Note that terminals are 1-indexed. This is an old terminal implementation
detail and not defined by Rawterm

* `[[nodiscard]] std::string set_terminal_title(const std::string&);`

* `void clear_screen();`
* `void clear_line();`
* `void clear_line_from();`
* `void clear_line_until();`
* `void clear_screen_until(const Pos);`
* `void clear_screen_from(const Pos);`
* `void clear_line(const Pos&);`
* `void clear_line_until(const Pos&);`
* `void clear_line_from(const Pos&);`
`from` starts at the cursor and goes to the end of the defined space 
whereas `until` goes from the start of the space up to the cursor.

* `[[nodiscard]] const std::string to_string(const Mod&);`
Return a string form of a Modifier key, to be used in conjunction
with rawterm::Key.getMod();


### Example
See `examples/` for more examples on using the library

```cpp
#include <rawterm/core.h>
#include <rawterm/cursor.h>
#include <rawterm/text.h>

int main() {
    // Raw mode automatically gets disabled at the end of the program
    rawterm::enable_raw_mode();

    rawterm::enter_alt_screen();

    // Create a cursor object to control the cursor, and reset it's position
    rawterm::Cursor c = rawterm::Cursor();
    c.reset();

    // Display text to the screen. This can be done the same as any other 
    // c++ text output. Rawterm provides a series of functions for customising
    // text appearance
    std::cout << rawterm::inverse("Hello world!\r\n");

    // Wait for user input
    rawterm::process_keypress();

    rawterm::exit_alt_screen();
    return 0;
}
```
