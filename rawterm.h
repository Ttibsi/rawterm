//////////////////////////////////////////////////////////////////////////////
// MIT License
//
// Copyright (c) 2023 Ttibsi
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
//////////////////////////////////////////////////////////////////////////////
// Code source: https://github.com/Ttibsi/rawterm/blob/main/rawterm.h
// Version: v1.7.1
//////////////////////////////////////////////////////////////////////////////

#ifndef RAWTERM_H
#define RAWTERM_H

#include <sys/ioctl.h>
#include <termios.h>
#include <unistd.h>

#include <algorithm>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <unordered_set>
#include <vector>

enum class Mod {
    Alt_L,
    Arrow,
    Backspace,
    Control,
    Delete,
    Escape,
    Function,
    Shift,
    Space,
    Unknown,
};

struct Key {
    char code;
    std::vector<Mod> mod;
    std::string raw;
};

struct Pos {
    int line;
    int col;
};

int enable_raw_mode();
void enter_alt_screen();
void exit_alt_screen();
Key process_keypress();

Pos get_term_size();
void clear_screen();

void move_cursor(Pos pos);
void save_cursor_position();
void load_cursor_position();
void cursor_block_blink();
void cursor_block();
void cursor_underscore_blink();
void cursor_underscore();
void cursor_pipe_blink();
void cursor_pipe();

std::string bold(const std::string& s);
std::string italic(const std::string& s);
std::string underline(const std::string& s);
std::string blink(const std::string& s);
std::string inverse(const std::string& s);
std::string hidden(const std::string& s);
std::string strikethrough(const std::string& s);

#endif // RAWTERM_H

#ifdef RAWTERM_IMPLEMENTATION

// enable/disable raw mode
// https://viewsourcecode.org/snaptoken/kilo/02.enteringRawMode.html

namespace detail {
    termios orig;

    void die(const char* s) {
        std::perror(s);
        std::exit(1);
    }

    std::unordered_set<char> asciiLetters {
        '\x31', '\x32', '\x33', '\x34', '\x35', '\x36', '\x37', '\x38',
        '\x39', '\x41', '\x42', '\x43', '\x44', '\x45', '\x46', '\x47',
        '\x48', '\x49', '\x4A', '\x4B', '\x4C', '\x4D', '\x4E', '\x4F',
        '\x50', '\x51', '\x52', '\x53', '\x54', '\x55', '\x56', '\x57',
        '\x58', '\x59', '\x5A', '\x61', '\x62', '\x63', '\x66', '\x67',
        '\x66', '\x67', '\x68', '\x69', '\x6A', '\x6B', '\x6C', '\x6D',
        '\x6E', '\x6F', '\x70', '\x71', '\x72', '\x73', '\x76', '\x77',
        '\x76', '\x77', '\x78', '\x79', '\x7A'
    };
}

// This doesn't need explicitly calling because of the atexit() call
void disable_raw_mode() {
    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &detail::orig) == -1)
        detail::die("tcsetattr");
}

int enable_raw_mode() {
    if (tcgetattr(STDIN_FILENO, &detail::orig) == -1)
        detail::die("tcgetattr");
    std::atexit(disable_raw_mode);

    termios raw = detail::orig;
    // raw.c_iflag &= ~(BRKINT | ICRNL | INPCK | ISTRIP | IXON);
    // raw.c_oflag &= ~(OPOST);
    // raw.c_lflag |= ~(CS8); // Disable to allow term_size reading
    // raw.c_lflag &= ~(ECHO | ICANON | IEXTEN | ISIG);

    // cfmakeraw can replace the above unless you want to allow configuring them individually
    // https://manpage.me/?q=cfmakeraw
    cfmakeraw(&raw);

    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw) == -1)
        detail::die("tcsetattr");

    return 0;
}

// Enter/leave alternate screen
// https://stackoverflow.com/a/12920036
// Will need to find another solution for windows (#ifdef WIN32)
void enter_alt_screen() { std::cout << "\x1b 7\x1b[?47h\x1b[H"; }

void exit_alt_screen() { std::cout << "\x1b[2J\x1b[?47l\x1b 8"; }

// Read user input
Key handle_escape(std::string_view characters, const std::string& raw) {
    // characters[0] is escape char
    // if characters[1] is equal to a letter (upper or lowercase), it's left
    // alt+letter if characters[1] is [, it's an arrow key (abcd after for udrl)
    // if characters[2] is \x31 (1), it can be f5+
    // f1 \x1b\x4f\x50
    // f2 \x1b\x4f\x51
    // f3 \x1b\x4f\x52
    // f4 \x1b\x4f\x53
    // f5 \x1b\x5b\x31\x35\x7e
    // f6 \x1b\x5b\x31\x37\x7e
    // f7 \x1b\x5b\x31\x38\x7e
    // f8 \x1b\x5b\x31\x39\x7e
    // f9 \x1b\x5b\x32\x30\x7e
    // f10 \x1b\x5b\x32\x31\x7e
    // f11 \x1b\x5b\x32\x33\x7e
    // f12 \x1b\x5b\x32\x34\x7e
    // delete: \x1b\x5b\x33\x7e

    if (characters.size() == 1) return {' ', {Mod::Escape}, raw}; // esc

    if (detail::asciiLetters.contains(characters[1])) {
        return {characters[1], {Mod::Alt_L}, raw};
    } else if (characters[1] == '\x5b') {
        // ARROWS
        switch (characters[2]) {
            case '\x41':
                return {'A', {Mod::Arrow}, raw}; // up
            case '\x42':
                return {'B', {Mod::Arrow}, raw}; // down
            case '\x43':
                return {'C', {Mod::Arrow}, raw}; // right
            case '\x44':
                return {'D', {Mod::Arrow}, raw}; // left
            case '\x33':
                return {' ', {Mod::Delete}, raw}; // delete

        // FUNCTIONS pt 2
            case '\x31':
                switch (characters[3]) {
                    case '\x35':
                        return {'5', {Mod::Function}, raw}; // f5
                    case '\x37':
                        return {'6', {Mod::Function}, raw}; // f6
                    case '\x38':
                        return {'7', {Mod::Function}, raw}; // f7
                    case '\x39':
                        return {'8', {Mod::Function}, raw}; // f8
                }
                break;
            case '\x32':
                switch (characters[3]) {
                    case '\x30':
                        return {'9', {Mod::Function}, raw}; // f9
                    case '\x31':
                        return {'0', {Mod::Function}, raw}; // f10
                    case '\x33':
                        return {'1', {Mod::Function}, raw}; // f11
                    case '\x34':
                        return {'2', {Mod::Function}, raw}; // f12
                }
                break;
        }
    } else if (characters[1] == '\x4f') {
        // FUNCTIONS pt 1
        switch (characters[2]) {
            case '\x50':
                return {'1', {Mod::Function}, raw}; // f1
            case '\x51':
                return {'2', {Mod::Function}, raw}; // f2
            case '\x52':
                return {'3', {Mod::Function}, raw}; // f3
            case '\x53':
                return {'4', {Mod::Function}, raw}; // f4
        }
    }

    return {' ', {Mod::Unknown}, raw};
}

Key process_keypress() {
    Key k;

    std::string characters;
    int ret = read(STDIN_FILENO, characters.data(), characters.size());
    if (ret < 0) {
        std::cerr
            << "ERROR: something went wrong during reading user input: "
            << std::strerror(errno) << std::endl;
        return k;
    }

    std::stringstream ss;
    ss << std::uppercase << std::hex << std::setw(2) << std::setfill('0');
    for (char c : characters) {
        ss << "\\x" << static_cast<int>(c);
    }
    k.raw = ss.str();

    // TODO: alt-gr, multiple modifier keys?
    // NOTE: enter/^m are the same entry
    // https://www.rapidtables.com/code/text/ascii-table.html
    switch (characters[0]) {
        case '\x01':
            k.code = 'a';
            k.mod.push_back(Mod::Control);
            break;
        case '\x02':
            k.code = 'b';
            k.mod.push_back(Mod::Control);
            break;
        case '\x03':
            k.code = 'c';
            k.mod.push_back(Mod::Control);
            break;
        case '\x04':
            k.code = 'd';
            k.mod.push_back(Mod::Control);
            break;
        case '\x05':
            k.code = 'e';
            k.mod.push_back(Mod::Control);
            break;
        case '\x06':
            k.code = 'f';
            k.mod.push_back(Mod::Control);
            break;
        case '\x07':
            k.code = 'g';
            k.mod.push_back(Mod::Control);
            break;
        case '\x08':
            k.code = 'h';
            k.mod.push_back(Mod::Control);
            break;
        case '\x09':
            k.code = 'i';
            k.mod.push_back(Mod::Control);
            break;
        case '\x0a':
            k.code = 'j';
            k.mod.push_back(Mod::Control);
            break;
        case '\x0b':
            k.code = 'k';
            k.mod.push_back(Mod::Control);
            break;
        case '\x0c':
            k.code = 'l';
            k.mod.push_back(Mod::Control);
            break;
        case '\x0d':
            k.code = 'm';
            k.mod.push_back(Mod::Control);
            break;
        case '\x0e':
            k.code = 'n';
            k.mod.push_back(Mod::Control);
            break;
        case '\x0f':
            k.code = 'o';
            k.mod.push_back(Mod::Control);
            break;
        case '\x10':
            k.code = 'p';
            k.mod.push_back(Mod::Control);
            break;
        case '\x11':
            k.code = 'q';
            k.mod.push_back(Mod::Control);
            break;
        case '\x12':
            k.code = 'r';
            k.mod.push_back(Mod::Control);
            break;
        case '\x13':
            k.code = 's';
            k.mod.push_back(Mod::Control);
            break;
        case '\x14':
            k.code = 't';
            k.mod.push_back(Mod::Control);
            break;
        case '\x15':
            k.code = 'u';
            k.mod.push_back(Mod::Control);
            break;
        case '\x16':
            k.code = 'v';
            k.mod.push_back(Mod::Control);
            break;
        case '\x17':
            k.code = 'w';
            k.mod.push_back(Mod::Control);
            break;
        case '\x18':
            k.code = 'x';
            k.mod.push_back(Mod::Control);
            break;
        case '\x19':
            k.code = 'y';
            k.mod.push_back(Mod::Control);
            break;
        case '\x1a':
            k.code = 'z';
            k.mod.push_back(Mod::Control);
            break;
        case '\x1b':
        // ESCAPE
            return handle_escape(characters, k.raw);
        case '\x1c':
            k.code = ' ';
            break;
        case '\x1d':
            k.code = ' ';
            break;
        case '\x1e':
            k.code = ' ';
            break;
        case '\x1f':
            k.code = ' ';
            break;
        case '\x20':
            k.code = ' ';
            k.mod.push_back(Mod::Space);
            break;
        case '\x21':
            k.code = '!';
            break;
        case '\x22':
            k.code = '"';
            break;
        case '\x23':
            k.code = '#';
            break;
        case '\x24':
            k.code = '$';
            break;
        case '\x25':
            k.code = '%';
            break;
        case '\x26':
            k.code = '&';
            break;
        case '\x27':
            k.code = '\'';
            break;
        case '\x28':
            k.code = '(';
            break;
        case '\x29':
            k.code = ')';
            break;
        case '\x2a':
            k.code = '*';
            break;
        case '\x2b':
            k.code = '+';
            break;
        case '\x2c':
            k.code = ',';
            break;
        case '\x2d':
            k.code = '-';
            break;
        case '\x2e':
            k.code = '.';
            break;
        case '\x2f':
            k.code = '/';
            break;
        case '\x30':
        // NUMBERS
            k.code = '0';
            break;
        case '\x31':
            k.code = '1';
            break;
        case '\x32':
            k.code = '2';
            break;
        case '\x33':
            k.code = '3';
            break;
        case '\x34':
            k.code = '4';
            break;
        case '\x35':
            k.code = '5';
            break;
        case '\x36':
            k.code = '6';
            break;
        case '\x37':
            k.code = '7';
            break;
        case '\x38':
            k.code = '8';
            break;
        case '\x39':
            k.code = '9';
            break;
        case '\x3a':
            k.code = ':';
            break;
        case '\x3b':
            k.code = ';';
            break;
        case '\x3c':
            k.code = '<';
            break;
        case '\x3d':
            k.code = '=';
            break;
        case '\x3e':
            k.code = '>';
            break;
        case '\x3f':
            k.code = '?';
            break;
        case '\x40':
            k.code = '@';
            break;
        case '\x41':
        // UPPERCASE LETTERS
            k.code = 'A';
            k.mod.push_back(Mod::Shift);
            break;
        case '\x42':
            k.code = 'B';
            k.mod.push_back(Mod::Shift);
            break;
        case '\x43':
            k.code = 'C';
            k.mod.push_back(Mod::Shift);
            break;
        case '\x44':
            k.code = 'D';
            k.mod.push_back(Mod::Shift);
            break;
        case '\x45':
            k.code = 'E';
            k.mod.push_back(Mod::Shift);
            break;
        case '\x46':
            k.code = 'F';
            k.mod.push_back(Mod::Shift);
            break;
        case '\x47':
            k.code = 'G';
            k.mod.push_back(Mod::Shift);
            break;
        case '\x48':
            k.code = 'H';
            k.mod.push_back(Mod::Shift);
            break;
        case '\x49':
            k.code = 'I';
            k.mod.push_back(Mod::Shift);
            break;
        case '\x4a':
            k.code = 'J';
            k.mod.push_back(Mod::Shift);
            break;
        case '\x4b':
            k.code = 'K';
            k.mod.push_back(Mod::Shift);
            break;
        case '\x4c':
            k.code = 'L';
            k.mod.push_back(Mod::Shift);
            break;
        case '\x4d':
            k.code = 'M';
            k.mod.push_back(Mod::Shift);
            break;
        case '\x4e':
            k.code = 'N';
            k.mod.push_back(Mod::Shift);
            break;
        case '\x4f':
            k.code = 'O';
            k.mod.push_back(Mod::Shift);
            break;
        case '\x50':
            k.code = 'P';
            k.mod.push_back(Mod::Shift);
            break;
        case '\x51':
            k.code = 'Q';
            k.mod.push_back(Mod::Shift);
            break;
        case '\x52':
            k.code = 'R';
            k.mod.push_back(Mod::Shift);
            break;
        case '\x53':
            k.code = 'S';
            k.mod.push_back(Mod::Shift);
            break;
        case '\x54':
            k.code = 'T';
            k.mod.push_back(Mod::Shift);
            break;
        case '\x55':
            k.code = 'U';
            k.mod.push_back(Mod::Shift);
            break;
        case '\x56':
            k.code = 'V';
            k.mod.push_back(Mod::Shift);
            break;
        case '\x57':
            k.code = 'W';
            k.mod.push_back(Mod::Shift);
            break;
        case '\x58':
            k.code = 'X';
            k.mod.push_back(Mod::Shift);
            break;
        case '\x59':
            k.code = 'Y';
            k.mod.push_back(Mod::Shift);
            break;
        case '\x5a':
            k.code = 'Z';
            k.mod.push_back(Mod::Shift);
            break;
        case '\x5b':
            k.code = '[';
            break;
        case '\x5c':
            k.code = '\\';
            break;
        case '\x5d':
            k.code = ']';
            break;
        case '\x5e':
            k.code = '^';
            break;
        case '\x5f':
            k.code = '_';
            break;
        case '\x60':
            k.code = '`';
            break;
        case '\x61':
        // LOWERCASE LETTERS
            k.code = 'a';
            break;
        case '\x62':
            k.code = 'b';
            break;
        case '\x63':
            k.code = 'c';
            break;
        case '\x64':
            k.code = 'd';
            break;
        case '\x65':
            k.code = 'e';
            break;
        case '\x66':
            k.code = 'f';
            break;
        case '\x67':
            k.code = 'g';
            break;
        case '\x68':
            k.code = 'h';
            break;
        case '\x69':
            k.code = 'i';
            break;
        case '\x6a':
            k.code = 'j';
            break;
        case '\x6b':
            k.code = 'k';
            break;
        case '\x6c':
            k.code = 'l';
            break;
        case '\x6d':
            k.code = 'm';
            break;
        case '\x6e':
            k.code = 'n';
            break;
        case '\x6f':
            k.code = 'o';
            break;
        case '\x70':
            k.code = 'p';
            break;
        case '\x71':
            k.code = 'q';
            break;
        case '\x72':
            k.code = 'r';
            break;
        case '\x73':
            k.code = 's';
            break;
        case '\x74':
            k.code = 't';
            break;
        case '\x75':
            k.code = 'u';
            break;
        case '\x76':
            k.code = 'v';
            break;
        case '\x77':
            k.code = 'w';
            break;
        case '\x78':
            k.code = 'x';
            break;
        case '\x79':
            k.code = 'y';
            break;
        case '\x7a':
            k.code = 'z';
            break;
        case '\x7b':
            k.code = '{';
            break;
        case '\x7c':
            k.code = '|';
            break;
        case '\x7d':
            k.code = '}';
            break;
        case '\x7e':
            k.code = '~';
            break;
        case '\x7f':
        // BACKSPACE
            k.code = ' ';
            k.mod.push_back(Mod::Backspace);
            break;
    }

    return k;
}

Pos get_term_size() {
    struct winsize w;
    ioctl(0, TIOCGWINSZ, &w);
    return Pos { w.ws_row, w.ws_col };
}
void clear_screen() { std::cout << "\x1b[2J"; }

void move_cursor(Pos pos) {
    std::cout << "\x1b[" << std::to_string(pos.line) << ";" << std::to_string(pos.col) << "H" << std::flush;
}
void save_cursor_position() { std::cout << "\x1b[s" << std::flush; }
void load_cursor_position() { std::cout << "\x1b[u" << std::flush; }
// https://stackoverflow.com/a/48449104
void cursor_block_blink() { std::cout << "\1\e[1 q\2" << std::flush; }
void cursor_block() { std::cout << "\1\e[2 q\2" << std::flush; }
void cursor_underscore_blink() { std::cout << "\1\e[3 q\2" << std::flush; }
void cursor_underscore() { std::cout << "\1\e[4 q\2" << std::flush; }
void cursor_pipe_blink() { std::cout << "\1\e[5 q\2" << std::flush; }
void cursor_pipe() { std::cout << "\1\e[6 q\6" << std::flush; }


// Text formatting
std::string bold(const std::string& s) { return "\x1b[1m" + s + "\x1b[22m"; }
std::string italic(const std::string& s) { return "\x1b[3m" + s + "\x1b[23m"; }
std::string underline(const std::string& s) { return "\x1b[4m" + s + "\x1b[24m"; }
std::string blink(const std::string& s) { return "\x1b[5m" + s + "\x1b[25m"; }
std::string inverse(const std::string& s) { return "\x1b[7m" + s + "\x1b[27m"; }
std::string hidden(const std::string& s) { return "\x1b[8m" + s + "\x1b[28m"; }
std::string strikethrough(const std::string& s) { return "\x1b[9m" + s + "\x1b[29m"; }

// Cursor positioning

#endif // RAWTERM_IMPLEMENTATION
