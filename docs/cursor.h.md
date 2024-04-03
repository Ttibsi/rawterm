# cursor.h
An object for tracking the state of the cursor in the terminal

##### methods

* `void reset();`
Move the cursor back to the coordinates saved in the Cursor object. Often 
useful if you need to move the raw terminal cursor with static methods 
listed below, or switch between cursor objects

* `void move(rawterm::Pos);`
* `void move_up(int);`
* `void move_down(int);`
* `void move_right(int);`
* `void move_left(int);`
These movement methods will take a number of the cells to move the cursor, with 
no bounds checking. Default value is 1. 

* `void offset_cursor(Pos);`

##### Static methods

* `static void save_cursor_position();`
* `static void load_cursor_position();`
These store and load the raw terminal cursor location in the terminal emulator's
memory. Supported by most terminals. This doesn't change the location stored 
within rawterm's memory. 

* `static void cursor_block_blink();`
* `static void cursor_block();`
* `static void cursor_underscore_blink();`
* `static void cursor_underscore();`
* `static void cursor_pipe_blink();`
* `static void cursor_pipe();`
* `static void cursor_hide();`
* `static void cursor_show();`
Change the appearance of the cursor the terminal displays. 

### Example

```cpp
#include <rawterm/cursor.h>

int main() {
    rawterm::Cursor cur = rawterm::Cursor();
    assert cur == rawterm::Pos{1,1};

    cur.move_down();
    cur.move_down();
    cur.move_right();
    assert cur == rawterm::Pos{3, 2};
}
