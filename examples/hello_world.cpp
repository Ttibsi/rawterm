#include <iostream>
#include "../rawterm.h"

int main() {
    // Raw mode automatically gets disabled at the end of the program
    rawterm::enable_raw_mode();

    rawterm::enter_alt_screen();

    // Set cursor to top left position
    rawterm::move_cursor({1,1});

    // Display text to the screen. This can be done the same as any other 
    // c++ text output. Rawterm provides a series of functions for customising
    // text appearance
    std::cout << rawterm::inverse("Hello world!\r\n");

    // Wait for user input
    rawterm::process_keypress();

    rawterm::exit_alt_screen();
    return 0;
}

