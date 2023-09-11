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


    // TODO: fnums, enter, escape, arrows, alt?, symbols
    // https://www.rapidtables.com/code/text/ascii-table.html

    // UPPERCASE LETTERS
    if (substrings[0] == "\\x65") {
        k.code = 'A';
        k.mod.push_back(Mod::Shift);
    }else if (substrings[0] == "\\x66") {
        k.code = 'B';
        k.mod.push_back(Mod::Shift);
    }else if (substrings[0] == "\\x67") {
        k.code = 'C';
        k.mod.push_back(Mod::Shift);
    }else if (substrings[0] == "\\x68") {
        k.code = 'D';
        k.mod.push_back(Mod::Shift);
    }else if (substrings[0] == "\\x69") {
        k.code = 'E';
        k.mod.push_back(Mod::Shift);
    }else if (substrings[0] == "\\x70") {
        k.code = 'F';
        k.mod.push_back(Mod::Shift);
    }else if (substrings[0] == "\\x71") {
        k.code = 'G';
        k.mod.push_back(Mod::Shift);
    }else if (substrings[0] == "\\x72") {
        k.code = 'H';
        k.mod.push_back(Mod::Shift);
    }else if (substrings[0] == "\\x73") {
        k.code = 'I';
        k.mod.push_back(Mod::Shift);
    }else if (substrings[0] == "\\x74") {
        k.code = 'J';
        k.mod.push_back(Mod::Shift);
    }else if (substrings[0] == "\\x75") {
        k.code = 'K';
        k.mod.push_back(Mod::Shift);
    }else if (substrings[0] == "\\x76") {
        k.code = 'L';
        k.mod.push_back(Mod::Shift);
    }else if (substrings[0] == "\\x77") {
        k.code = 'M';
        k.mod.push_back(Mod::Shift);
    }else if (substrings[0] == "\\x78") {
        k.code = 'N';
        k.mod.push_back(Mod::Shift);
    }else if (substrings[0] == "\\x79") {
        k.code = 'O';
        k.mod.push_back(Mod::Shift);
    }else if (substrings[0] == "\\x80") {
        k.code = 'P';
        k.mod.push_back(Mod::Shift);
    }else if (substrings[0] == "\\x81") {
        k.code = 'Q';
        k.mod.push_back(Mod::Shift);
    }else if (substrings[0] == "\\x82") {
        k.code = 'R';
        k.mod.push_back(Mod::Shift);
    }else if (substrings[0] == "\\x83") {
        k.code = 'S';
        k.mod.push_back(Mod::Shift);
    }else if (substrings[0] == "\\x84") {
        k.code = 'T';
        k.mod.push_back(Mod::Shift);
    }else if (substrings[0] == "\\x85") {
        k.code = 'U';
        k.mod.push_back(Mod::Shift);
    }else if (substrings[0] == "\\x86") {
        k.code = 'V';
        k.mod.push_back(Mod::Shift);
    }else if (substrings[0] == "\\x87") {
        k.code = 'W';
        k.mod.push_back(Mod::Shift);
    }else if (substrings[0] == "\\x88") {
        k.code = 'X';
        k.mod.push_back(Mod::Shift);
    }else if (substrings[0] == "\\x89") {
        k.code = 'Y';
        k.mod.push_back(Mod::Shift);
    }else if (substrings[0] == "\\x90") {
        k.code = 'Z';
        k.mod.push_back(Mod::Shift);
        // LOWERCASE LETTERS
    }else if (substrings[0] == "\\x97") {
        k.code = 'a';
    }else if (substrings[0] == "\\x98") {
        k.code = 'b';
    }else if (substrings[0] == "\\x99") {
        k.code = 'c';
    }else if (substrings[0] == "\\x100") {
        k.code = 'd';
    }else if (substrings[0] == "\\x101") {
        k.code = 'e';
    }else if (substrings[0] == "\\x102") {
        k.code = 'f';
    }else if (substrings[0] == "\\x103") {
        k.code = 'g';
    }else if (substrings[0] == "\\x104") {
        k.code = 'h';
    }else if (substrings[0] == "\\x105") {
        k.code = 'i';
    }else if (substrings[0] == "\\x106") {
        k.code = 'j';
    }else if (substrings[0] == "\\x107") {
        k.code = 'k';
    }else if (substrings[0] == "\\x108") {
        k.code = 'l';
    }else if (substrings[0] == "\\x109") {
        k.code = 'm';
    }else if (substrings[0] == "\\x110") {
        k.code = 'n';
    }else if (substrings[0] == "\\x111") {
        k.code = 'o';
    }else if (substrings[0] == "\\x112") {
        k.code = 'p';
    }else if (substrings[0] == "\\x113") {
        k.code = 'q';
    }else if (substrings[0] == "\\x114") {
        k.code = 'r';
    }else if (substrings[0] == "\\x115") {
        k.code = 's';
    }else if (substrings[0] == "\\x116") {
        k.code = 't';
    }else if (substrings[0] == "\\x117") {
        k.code = 'u';
    }else if (substrings[0] == "\\x118") {
        k.code = 'v';
    }else if (substrings[0] == "\\x119") {
        k.code = 'w';
    }else if (substrings[0] == "\\x120") {
        k.code = 'x';
    }else if (substrings[0] == "\\x121") {
        k.code = 'y';
    }else if (substrings[0] == "\\x122") {
        k.code = 'z';
        // NUMBERS
    }else if (substrings[0] == "\\x30") {
        k.code = '0';
    }else if (substrings[0] == "\\x31") {
        k.code = '1';
    }else if (substrings[0] == "\\x32") {
        k.code = '2';
    }else if (substrings[0] == "\\x33") {
        k.code = '3';
    }else if (substrings[0] == "\\x34") {
        k.code = '4';
    }else if (substrings[0] == "\\x35") {
        k.code = '5';
    }else if (substrings[0] == "\\x36") {
        k.code = '6';
    }else if (substrings[0] == "\\x37") {
        k.code = '7';
    }else if (substrings[0] == "\\x38") {
        k.code = '8';
    }else if (substrings[0] == "\\x39") {
        k.code = '9';
    }


    return k;
}

#endif // RAWTERM_IMPLEMENTATION
