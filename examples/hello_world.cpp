#include "rawterm/color.h"
#include <iostream>

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

    // Set the background colour to grey
    rawterm::set_terminal_background(rawterm::Colors::black);

    // Display text to the screen. This can be done the same as any other 
    // c++ text output. Rawterm provides a series of functions for customising
    // text appearance
    std::cout << rawterm::inverse("Hello world!\r\n");

    // Wait for user input
    auto k = rawterm::process_keypress();

    rawterm::exit_alt_screen();
    return 0;
}

