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
// Version: v3.0.0
//////////////////////////////////////////////////////////////////////////////

#ifndef RAWTERM_CORE_H
#define RAWTERM_CORE_H

#include <cctype>
#include <cmath>
#include <csignal>
#include <cstddef>
#include <cstdint>
#include <deque>
#include <functional>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <unistd.h>
#include <unordered_set>
#include <vector>

#if __linux__
    #include <sys/ioctl.h>
    #include <termios.h>
#elif _WIN32
    #define WINDOWS_LEAN_AND_MEAN
    #include <windows.h>
#endif


// enable/disable raw mode
// https://viewsourcecode.org/snaptoken/kilo/02.enteringRawMode.html

namespace rawterm {

    namespace detail {
        #if __linux__
            inline termios orig;
        #endif
        inline bool sigtstp_called = false;
        inline bool sigcont_called = false;
    } // namespace detail

    enum struct Mod {
        Alt_L,
        Arrow,
        Backspace,
        Control,
        Delete,
        Enter,
        Escape,
        Function,
        None,
        Shift,
        Space,
        Tab, 
        Unknown
    };

    struct Key {
        char code;
        std::deque<rawterm::Mod> mod;
        std::string raw;

        rawterm::Mod getMod();
        bool isCharInput();
    };

    // 0 == vertical == ^v, 1 == horizontal == <>
    struct Pos {
        int vertical;
        int horizontal;

        friend constexpr bool operator==(const Pos& self, const Pos& other) {
            return self.vertical == other.vertical && self.horizontal == other.horizontal;
        }
    };

    // TODO: constexpr std array
    static const std::unordered_set<char> asciiLetters{
        '\x31', '\x32', '\x33', '\x34', '\x35', '\x36', '\x37', '\x38', '\x39',
        '\x41', '\x42', '\x43', '\x44', '\x45', '\x46', '\x47', '\x48', '\x49',
        '\x4A', '\x4B', '\x4C', '\x4D', '\x4E', '\x4F', '\x50', '\x51', '\x52',
        '\x53', '\x54', '\x55', '\x56', '\x57', '\x58', '\x59', '\x5A', '\x61',
        '\x62', '\x63', '\x64', '\x65', '\x66', '\x67', '\x68', '\x69', '\x6A',
        '\x6B', '\x6C', '\x6D', '\x6E', '\x6F', '\x70', '\x71', '\x72', '\x73',
        '\x74', '\x75', '\x76', '\x77', '\x78', '\x79', '\x7A'
    };

    void disable_raw_mode();
    int enable_raw_mode();
    void enter_alt_screen();
    void exit_alt_screen();
    void enable_signals();
    void sigtstp_handler(std::function<void(void)>);
    void sigcont_handler(std::function<void(void)>);
    rawterm::Key process_keypress();
    rawterm::Pos get_term_size();
    void clear_screen();
    void clear_line();
    void clear_line_from();
    void clear_line_until();
    void clear_screen_until(const Pos);
    void clear_screen_from(const Pos);
    void clear_line(const Pos&);
    void clear_line_until(const Pos&);
    void clear_line_from(const Pos&);
    const std::string to_string(const Mod& );

} // namespace rawterm

#endif // RAWTERM_CORE_H
