#ifndef RAWTERM_H
#define RAWTERM_H

#include <stdlib.h>
#include <termios.h>
#include <unistd.h>

int hello();
int enable_raw_mode();

#endif // RAWTERM_H

#ifdef RAWTERM_IMPLEMENTATION

int hello() {
    return 1;
}

// enable/disable raw mode
// https://viewsourcecode.org/snaptoken/kilo/02.enteringRawMode.html

struct termios orig;

// This doesn't need explicitly calling because of the atexit() call
void disable_raw_mode() {
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig);
}

int enable_raw_mode() {
    tcgetattr(STDIN_FILENO, &orig);
    atexit(disable_raw_mode);

    struct termios raw = orig;
    raw.c_lflag &= ~(ECHO | ICANON);

    tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);

    return 0;
}



// Enter/leave alternate screen
// https://stackoverflow.com/a/12920036
// Will need to find another solution for windows (#ifdef WIN32)

// Read user input - I want to return a struct that holds the value and any 
// modifier keys pressed - CTRL/CMD/SHIFT/ALT/SUPER

#endif // RAWTERM_IMPLEMENTATION
