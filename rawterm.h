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

void exit_alt_screen() {
    std::cout << "\033[2J\033[?47l\0338";
}

// Read user input
enum Mod { 
    Control,
    Shift,
    Escape,
    Arrow,
};

struct Key {
    char code;
    std::vector<Mod> mod;
    int raw;
};

// Key process_keypress() {
//     char c;
//     std::cin.get(c);
//
//     Key k;
//     k.raw = static_cast<int>(c);
//
//     // TODO: Escape, backspace, enter
//     if (c == 27) {
//         char next;
//         if (std::cin.get(next)) {
//             if (next == '[') {
//                 char escape_char;
//                 if (std::cin.get(escape_char)) {
//                     k.mod.push_back(Mod::Arrow);
//
//                     if (escape_char == 'A') {
//                         k.code = 'a'; // Up arrow key
//                     } else if (escape_char == 'B') {
//                         k.code = 'b'; // Down arrow key
//                     } else if (escape_char == 'C') {
//                         k.code = 'c'; // Right arrow key
//                     } else if (escape_char == 'D') {
//                         k.code = 'd'; // Left arrow key
//                     }
//                 }
//             }
//         } else {
//             k.code = ' ';
//             k.mod.push_back(Mod::Escape);
//         }
//     } else if (std::isupper(c)) {
//         k.code = static_cast<int>(c) + 32;
//         k.mod.push_back(Mod::Shift);
//     } else if (std::iscntrl(c)) {
//         k.code = static_cast<int>(c) + 96;
//         k.mod.push_back(Mod::Control);
//     } else {
//         k.code = c;
//     }
//
//     return k;
// }



Key process_keypress() {
    Key k;
    char seq[32];

    int ret = read(STDIN_FILENO, seq, sizeof(seq));
    if (ret < 0) {
        std::cerr << "ERROR: something went wrong during reading user input: " << std::strerror(errno) << std::endl;
        return k;
    }

    std::string code;
    for (int i = 0; i < ret; ++i) {
        char buffer[5]; // Enough space for "\\x00" plus null-terminator
        std::snprintf(buffer, sizeof(buffer), "\\x%02x", static_cast<unsigned char>(seq[i]));
        code += buffer;
    }

    std::vector<std::string> substrings;
    for (size_t i = 0; i < code.length(); i += 4) {
        // Get a substring of 4 characters
        std::string sub = code.substr(i, 4);
        substrings.push_back(sub);
    }


    // https://www.rapidtables.com/code/text/ascii-table.html
    // Escape
    std::cout << "sub0" << substrings[0] << "\r\n";
    if (substrings[0] == "\x27") {
    } else if (substrings[0] == "\\x61") {
        std::cout << "aaaaaaa\r\n";
    }


    return k;
}

#endif // RAWTERM_IMPLEMENTATION
