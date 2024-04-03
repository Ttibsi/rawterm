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
* `void enter_alt_screen();`
* `void exit_alt_screen();`
* `void enable_signals();`
* `void sigtstp_handler(std::function<void(void)>);`
* `void sigcont_handler(std::function<void(void)>);`
* `[[nodiscard]] rawterm::Key process_keypress();`
* `[[nodiscard]] rawterm::Pos get_term_size();`
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
* `[[nodiscard]] const std::string to_string(const Mod&);`
