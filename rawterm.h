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

// enable/disable raw mode
// https://viewsourcecode.org/snaptoken/kilo/02.enteringRawMode.html

namespace rawterm {
	namespace detail {
		inline termios orig;

		inline void die(const char* s) {
			std::perror(s);
			std::exit(1);
		}
	} // namespace detail

	enum struct Mod {
		Alt_L,
		Arrow,
		Backspace,
		Control,
		Delete,
		Escape,
		Function,
		Shift,
		Space,
		Unknown
	};

	struct Key {
		char code;
		std::vector<rawterm::Mod> mod;
		std::string raw;
	};

	struct Pos {
		int line;
		int col;
	};

	// Enter/leave alternate screen
	// https://stackoverflow.com/a/12920036
	// Will need to find another solution for windows (#ifdef WIN32)

	// This doesn't need explicitly calling because of the atexit() call
	inline void disable_raw_mode() {
		if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &rawterm::detail::orig) == -1) {
			rawterm::detail::die("tcsetattr");
		}
	}

	inline void enable_raw_mode() {
		if (tcgetattr(STDIN_FILENO, &rawterm::detail::orig) == -1) {
			rawterm::detail::die("tcgetattr");
		}
		std::atexit(rawterm::disable_raw_mode);

		termios raw = rawterm::detail::orig;
		cfmakeraw(&raw);

		if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw) == -1) {
			rawterm::detail::die("tcsetattr");
		}
	}

	inline void enter_alt_screen() {
		std::cout << "\x1B 7\x1B[?47h\x1B[H";
	}

	inline void exit_alt_screen() {
		std::cout << "\x1B[2J\x1B[?47l\x1B 8";
	}

	// Read user input
	inline rawterm::Key process_keypress() {
		static const std::unordered_set<char> asciiLetters {
			'\x31', '\x32', '\x33', '\x34', '\x35', '\x36', '\x37', '\x38',
			'\x39', '\x41', '\x42', '\x43', '\x44', '\x45', '\x46', '\x47',
			'\x48', '\x49', '\x4A', '\x4B', '\x4C', '\x4D', '\x4E', '\x4F',
			'\x50', '\x51', '\x52', '\x53', '\x54', '\x55', '\x56', '\x57',
			'\x58', '\x59', '\x5A', '\x61', '\x62', '\x63', '\x66', '\x67',
			'\x66', '\x67', '\x68', '\x69', '\x6A', '\x6B', '\x6C', '\x6D',
			'\x6E', '\x6F', '\x70', '\x71', '\x72', '\x73', '\x76', '\x77',
			'\x76', '\x77', '\x78', '\x79', '\x7A'
		};

		char buffer[5];
		const ssize_t ret = read(STDIN_FILENO, buffer, 5);
		if (ret < 0) {
			std::cerr
				<< "ERROR: something went wrong during reading user input: "
				<< std::strerror(errno) << std::endl;
			return {' ', {rawterm::Mod::Unknown}, ""};
		}
		const std::string characters = buffer;

		std::stringstream ss;
		ss << std::uppercase << std::hex << std::setw(2) << std::setfill('0');
		for (char c : characters) {
			ss << "\\x" << static_cast<int>(static_cast<unsigned char>(c));
		}
		const std::string raw = ss.str();

		// TODO: alt-gr, multiple modifier keys?
		// NOTE: enter/^m are the same entry
		// https://www.rapidtables.com/code/text/ascii-table.html
		switch (characters[0]) {
			case '\x01':
				return {'a', {rawterm::Mod::Control}, raw};
			case '\x02':
				return {'b', {rawterm::Mod::Control}, raw};
			case '\x03':
				return {'c', {rawterm::Mod::Control}, raw};
			case '\x04':
				return {'d', {rawterm::Mod::Control}, raw};
			case '\x05':
				return {'e', {rawterm::Mod::Control}, raw};
			case '\x06':
				return {'f', {rawterm::Mod::Control}, raw};
			case '\x07':
				return {'g', {rawterm::Mod::Control}, raw};
			case '\x08':
				return {'h', {rawterm::Mod::Control}, raw};
			case '\x09':
				return {'i', {rawterm::Mod::Control}, raw};
			case '\x0A':
				return {'j', {rawterm::Mod::Control}, raw};
			case '\x0B':
				return {'k', {rawterm::Mod::Control}, raw};
			case '\x0C':
				return {'l', {rawterm::Mod::Control}, raw};
			case '\x0D':
				return {'m', {rawterm::Mod::Control}, raw};
			case '\x0E':
				return {'n', {rawterm::Mod::Control}, raw};
			case '\x0F':
				return {'o', {rawterm::Mod::Control}, raw};
			case '\x10':
				return {'p', {rawterm::Mod::Control}, raw};
			case '\x11':
				return {'q', {rawterm::Mod::Control}, raw};
			case '\x12':
				return {'r', {rawterm::Mod::Control}, raw};
			case '\x13':
				return {'s', {rawterm::Mod::Control}, raw};
			case '\x14':
				return {'t', {rawterm::Mod::Control}, raw};
			case '\x15':
				return {'u', {rawterm::Mod::Control}, raw};
			case '\x16':
				return {'v', {rawterm::Mod::Control}, raw};
			case '\x17':
				return {'w', {rawterm::Mod::Control}, raw};
			case '\x18':
				return {'x', {rawterm::Mod::Control}, raw};
			case '\x19':
				return {'y', {rawterm::Mod::Control}, raw};
			case '\x1A':
				return {'z', {rawterm::Mod::Control}, raw};
			case '\x1B':
				// ESCAPE

				// characters[0] is escape char
				// if characters[1] is equal to a letter (upper or lowercase), it's left
				// alt+letter if characters[1] is [, it's an arrow key (abcd after for udrl)
				// if characters[2] is \x31 (1), it can be f5+
				// f1 \x1B\x4F\x50
				// f2 \x1B\x4F\x51
				// f3 \x1B\x4F\x52
				// f4 \x1B\x4F\x53
				// f5 \x1B\x5B\x31\x35\x7E
				// f6 \x1B\x5B\x31\x37\x7E
				// f7 \x1B\x5B\x31\x38\x7E
				// f8 \x1B\x5B\x31\x39\x7E
				// f9 \x1B\x5B\x32\x30\x7E
				// f10 \x1B\x5B\x32\x31\x7E
				// f11 \x1B\x5B\x32\x33\x7E
				// f12 \x1B\x5B\x32\x34\x7E
				// delete: \x1B\x5B\x33\x7E

				if (characters.size() == 1) {
					return {' ', {rawterm::Mod::Escape}, raw}; // esc
				}
				if (asciiLetters.contains(characters[1])) {
					return {characters[1], {rawterm::Mod::Alt_L}, raw};
				}
				if (characters[1] == '\x5B') {
					// ARROWS
					switch (characters[2]) {
						case '\x41':
							return {'A', {rawterm::Mod::Arrow}, raw}; // up
						case '\x42':
							return {'B', {rawterm::Mod::Arrow}, raw}; // down
						case '\x43':
							return {'C', {rawterm::Mod::Arrow}, raw}; // right
						case '\x44':
							return {'D', {rawterm::Mod::Arrow}, raw}; // left
						case '\x33':
							return {' ', {rawterm::Mod::Delete}, raw}; // delete

						// FUNCTIONS pt 2
						case '\x31':
							switch (characters[3]) {
								case '\x35':
									return {'5', {rawterm::Mod::Function}, raw}; // f5
								case '\x37':
									return {'6', {rawterm::Mod::Function}, raw}; // f6
								case '\x38':
									return {'7', {rawterm::Mod::Function}, raw}; // f7
								case '\x39':
									return {'8', {rawterm::Mod::Function}, raw}; // f8
							}
							break;
						case '\x32':
							switch (characters[3]) {
								case '\x30':
									return {'9', {rawterm::Mod::Function}, raw}; // f9
								case '\x31':
									return {'0', {rawterm::Mod::Function}, raw}; // f10
								case '\x33':
									return {'1', {rawterm::Mod::Function}, raw}; // f11
								case '\x34':
									return {'2', {rawterm::Mod::Function}, raw}; // f12
							}
							break;
					}
				} else if (characters[1] == '\x4F') {
					// FUNCTIONS pt 1
					switch (characters[2]) {
						case '\x50':
							return {'1', {rawterm::Mod::Function}, raw}; // f1
						case '\x51':
							return {'2', {rawterm::Mod::Function}, raw}; // f2
						case '\x52':
							return {'3', {rawterm::Mod::Function}, raw}; // f3
						case '\x53':
							return {'4', {rawterm::Mod::Function}, raw}; // f4
					}
				}
				break;
			case '\x1C':
				return {' ', {}, raw};
			case '\x1D':
				return {' ', {}, raw};
			case '\x1E':
				return {' ', {}, raw};
			case '\x1F':
				return {' ', {}, raw};
			case '\x20':
				return {' ', {rawterm::Mod::Space}, raw};
			case '\x21':
				return {'!', {}, raw};
			case '\x22':
				return {'"', {}, raw};
			case '\x23':
				return {'#', {}, raw};
			case '\x24':
				return {'$', {}, raw};
			case '\x25':
				return {'%', {}, raw};
			case '\x26':
				return {'&', {}, raw};
			case '\x27':
				return {'\'', {}, raw};
			case '\x28':
				return {'(', {}, raw};
			case '\x29':
				return {')', {}, raw};
			case '\x2A':
				return {'*', {}, raw};
			case '\x2B':
				return {'+', {}, raw};
			case '\x2C':
				return {',', {}, raw};
			case '\x2D':
				return {'-', {}, raw};
			case '\x2E':
				return {'.', {}, raw};
			case '\x2F':
				return {'/', {}, raw};
			case '\x30':
				// NUMBERS
				return {'0', {}, raw};
			case '\x31':
				return {'1', {}, raw};
			case '\x32':
				return {'2', {}, raw};
			case '\x33':
				return {'3', {}, raw};
			case '\x34':
				return {'4', {}, raw};
			case '\x35':
				return {'5', {}, raw};
			case '\x36':
				return {'6', {}, raw};
			case '\x37':
				return {'7', {}, raw};
			case '\x38':
				return {'8', {}, raw};
			case '\x39':
				return {'9', {}, raw};
			case '\x3A':
				return {':', {}, raw};
			case '\x3B':
				return {';', {}, raw};
			case '\x3C':
				return {'<', {}, raw};
			case '\x3D':
				return {'=', {}, raw};
			case '\x3E':
				return {'>', {}, raw};
			case '\x3F':
				return {'?', {}, raw};
			case '\x40':
				return {'@', {}, raw};
			case '\x41':
				// UPPERCASE LETTERS
				return {'A', {rawterm::Mod::Shift}, raw};
			case '\x42':
				return {'B', {rawterm::Mod::Shift}, raw};
			case '\x43':
				return {'C', {rawterm::Mod::Shift}, raw};
			case '\x44':
				return {'D', {rawterm::Mod::Shift}, raw};
			case '\x45':
				return {'E', {rawterm::Mod::Shift}, raw};
			case '\x46':
				return {'F', {rawterm::Mod::Shift}, raw};
			case '\x47':
				return {'G', {rawterm::Mod::Shift}, raw};
			case '\x48':
				return {'H', {rawterm::Mod::Shift}, raw};
			case '\x49':
				return {'I', {rawterm::Mod::Shift}, raw};
			case '\x4A':
				return {'J', {rawterm::Mod::Shift}, raw};
			case '\x4B':
				return {'K', {rawterm::Mod::Shift}, raw};
			case '\x4C':
				return {'L', {rawterm::Mod::Shift}, raw};
			case '\x4D':
				return {'M', {rawterm::Mod::Shift}, raw};
			case '\x4E':
				return {'N', {rawterm::Mod::Shift}, raw};
			case '\x4F':
				return {'O', {rawterm::Mod::Shift}, raw};
			case '\x50':
				return {'P', {rawterm::Mod::Shift}, raw};
			case '\x51':
				return {'Q', {rawterm::Mod::Shift}, raw};
			case '\x52':
				return {'R', {rawterm::Mod::Shift}, raw};
			case '\x53':
				return {'S', {rawterm::Mod::Shift}, raw};
			case '\x54':
				return {'T', {rawterm::Mod::Shift}, raw};
			case '\x55':
				return {'U', {rawterm::Mod::Shift}, raw};
			case '\x56':
				return {'V', {rawterm::Mod::Shift}, raw};
			case '\x57':
				return {'W', {rawterm::Mod::Shift}, raw};
			case '\x58':
				return {'X', {rawterm::Mod::Shift}, raw};
			case '\x59':
				return {'Y', {rawterm::Mod::Shift}, raw};
			case '\x5A':
				return {'Z', {rawterm::Mod::Shift}, raw};
			case '\x5B':
				return {'[', {}, raw};
			case '\x5C':
				return {'\\', {}, raw};
			case '\x5D':
				return {']', {}, raw};
			case '\x5E':
				return {'^', {}, raw};
			case '\x5F':
				return {'_', {}, raw};
			case '\x60':
				return {'`', {}, raw};
			case '\x61':
				// LOWERCASE LETTERS
				return {'a', {}, raw};
			case '\x62':
				return {'b', {}, raw};
			case '\x63':
				return {'c', {}, raw};
			case '\x64':
				return {'d', {}, raw};
			case '\x65':
				return {'e', {}, raw};
			case '\x66':
				return {'f', {}, raw};
			case '\x67':
				return {'g', {}, raw};
			case '\x68':
				return {'h', {}, raw};
			case '\x69':
				return {'i', {}, raw};
			case '\x6A':
				return {'j', {}, raw};
			case '\x6B':
				return {'k', {}, raw};
			case '\x6C':
				return {'l', {}, raw};
			case '\x6D':
				return {'m', {}, raw};
			case '\x6E':
				return {'n', {}, raw};
			case '\x6F':
				return {'o', {}, raw};
			case '\x70':
				return {'p', {}, raw};
			case '\x71':
				return {'q', {}, raw};
			case '\x72':
				return {'r', {}, raw};
			case '\x73':
				return {'s', {}, raw};
			case '\x74':
				return {'t', {}, raw};
			case '\x75':
				return {'u', {}, raw};
			case '\x76':
				return {'v', {}, raw};
			case '\x77':
				return {'w', {}, raw};
			case '\x78':
				return {'x', {}, raw};
			case '\x79':
				return {'y', {}, raw};
			case '\x7A':
				return {'z', {}, raw};
			case '\x7B':
				return {'{', {}, raw};
			case '\x7C':
				return {'|', {}, raw};
			case '\x7D':
				return {'}', {}, raw};
			case '\x7E':
				return {'~', {}, raw};
			case '\x7F':
				// BACKSPACE
				return {' ', {rawterm::Mod::Backspace}, raw};
		}

		return {' ', {rawterm::Mod::Unknown}, raw};
	}

	inline rawterm::Pos get_term_size() {
		struct winsize w;
		ioctl(0, TIOCGWINSZ, &w);
		return Pos { w.ws_row, w.ws_col };
	}

	inline void clear_screen() {
		std::cout << "\x1B[2J";
	}

	inline void move_cursor(rawterm::Pos pos) {
		std::cout << "\x1B[" << std::to_string(pos.line) << ';' << std::to_string(pos.col) << 'H' << std::flush;
	}

	inline void save_cursor_position() {
		std::cout << "\x1B[s" << std::flush;
	}

	inline void load_cursor_position() {
		std::cout << "\x1B[u" << std::flush;
	}

	// https://stackoverflow.com/a/48449104
	inline void cursor_block_blink() {
		std::cout << "\1\x1B[1 q\2" << std::flush;
	}

	inline void cursor_block() {
		std::cout << "\1\x1B[2 q\2" << std::flush;
	}

	inline void cursor_underscore_blink() {
		std::cout << "\1\x1B[3 q\2" << std::flush;
	}

	inline void cursor_underscore() {
		std::cout << "\1\x1B[4 q\2" << std::flush;
	}

	inline void cursor_pipe_blink() {
		std::cout << "\1\x1B[5 q\2" << std::flush;
	}

	inline void cursor_pipe() {
		std::cout << "\1\x1B[6 q\6" << std::flush;
	}

	inline std::string bold(const std::string& s) {
		return "\x1B[1m" + s + "\x1B[22m";
	}

	inline std::string italic(const std::string& s) {
		return "\x1B[3m" + s + "\x1B[23m";
	}

	inline std::string underline(const std::string& s) {
		return "\x1B[4m" + s + "\x1B[24m";
	}

	inline std::string blink(const std::string& s) {
		return "\x1B[5m" + s + "\x1B[25m";
	}

	inline std::string inverse(const std::string& s) {
		return "\x1B[7m" + s + "\x1B[27m";
	}

	inline std::string hidden(const std::string& s) {
		return "\x1B[8m" + s + "\x1B[28m";
	}

	inline std::string strikethrough(const std::string& s) {
		return "\x1B[9m" + s + "\x1B[29m";
	}
} // namespace rawterm

#endif // RAWTERM_H
