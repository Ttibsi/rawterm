#ifndef RAWTERM_H
#define RAWTERM_H

#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <termios.h>
#include <unistd.h>
#include <vector>

int enable_raw_mode();

#endif // RAWTERM_H

#ifdef RAWTERM_IMPLEMENTATION

// enable/disable raw mode
// https://viewsourcecode.org/snaptoken/kilo/02.enteringRawMode.html

struct termios orig;

void die(const char* s) {
    std::perror(s);
    std::exit(1);
}

// This doesn't need explicitly calling because of the atexit() call
void disable_raw_mode() {
    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig) == -1) die("tcsetattr");
}

int enable_raw_mode() {
    if (tcgetattr(STDIN_FILENO, &orig) == -1) die("tcgetattr");
    atexit(disable_raw_mode);

    struct termios raw = orig;
    raw.c_iflag &= ~(BRKINT | ICRNL | INPCK | ISTRIP | IXON);
    raw.c_oflag &= ~(OPOST);
    raw.c_lflag |= ~(CS8);
    raw.c_lflag &= ~(ECHO | ICANON | IEXTEN | ISIG);

    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw) == -1) die("tcsetattr");

    return 0;
}

// Enter/leave alternate screen
// https://stackoverflow.com/a/12920036
// Will need to find another solution for windows (#ifdef WIN32)
void enter_alt_screen() {
    std::cout << "\0337\033[?47h\033[H";
}

void exitr_alt_screen() {
    std::cout << "\033[2J\033[?47l\0338";
}

// Read user input
enum Mod { 
    Control,
    Shift
};

struct Key {
    char code;
    std::vector<Mod> mod;
};

Key process_keypress() {
    char c;
    std::cin.get(c);

    Key k;

    if (65 <= static_cast<int>(c) && static_cast<int>(c) <= 90) {
        k.code = static_cast<int>(c) + 32;
        k.mod.push_back(Mod::Shift);
    } else if (std::iscntrl(c)) {
        k.code = static_cast<int>(c);
        k.mod.push_back(Mod::Control);
    } else {
        k.code = c;
    }

    return k;
}

#endif // RAWTERM_IMPLEMENTATION
