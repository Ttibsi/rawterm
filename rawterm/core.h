#ifndef RAWTERM_CORE_H
#define RAWTERM_CORE_H

#include <poll.h>
#include <unistd.h>

#include <array>
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
#include <unordered_set>
#include <utility>
#include <vector>

#if __linux__
#include <sys/ioctl.h>
#include <termios.h>
#elif _WIN32
#define WINDOWS_LEAN_AND_MEAN
#include <windows.h>
#endif

namespace rawterm {

    namespace detail {
#if __linux__
        inline termios orig;
#endif
        inline bool sigtstp_called = false;
        inline bool sigcont_called = false;

        // Used for polling in process_keypress()
        inline pollfd fd {STDIN_FILENO, POLLIN, POLLOUT};

        // used for debugging
        [[nodiscard]] bool is_debug();
    }  // namespace detail

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

        Key(char c, Mod m) : code(c), mod(std::deque<Mod> {m}) {}
        Key(char c, Mod m, std::string r) : code(c), mod(std::deque<Mod> {m}), raw(r) {}

        [[nodiscard]] rawterm::Mod getMod();
        [[nodiscard]] bool isCharInput();
        [[nodiscard]] bool isValid();

        [[nodiscard]] bool operator==(const Key& other) const {
            if (other.mod.empty() || other.mod == std::deque<Mod> {Mod::None}) {
                // None is the same as empty
                return this->code == other.code &&
                       (this->mod == std::deque<Mod> {Mod::None} || this->mod.empty());
            } else {
                return this->code == other.code && this->mod == other.mod;
            }
        }
    };

    // 0 == vertical == ^v, 1 == horizontal == <>
    struct Pos {
        int vertical;
        int horizontal;

        [[nodiscard]] constexpr bool partial_cmp(const Pos& rhs) {
            return this->vertical == rhs.vertical || this->horizontal == rhs.horizontal;
        }

        friend std::ostream& operator<<(std::ostream& os, const Pos& p) {
            return os << "Pos{" << p.vertical << ", " << p.horizontal << "}";
        }

        [[nodiscard]] constexpr bool operator==(const Pos& other) const {
            return this->vertical == other.vertical && this->horizontal == other.horizontal;
        }

        constexpr Pos& operator+=(const Pos& other) {
            this->vertical += other.vertical;
            this->horizontal += other.horizontal;
            return *this;
        }

        [[nodiscard]] constexpr Pos operator+(const Pos& other) const {
            return {this->vertical + other.vertical, this->horizontal + other.horizontal};
        }

        constexpr Pos& operator+=(const int other) {
            this->vertical += other;
            this->horizontal += other;
            return *this;
        }

        constexpr Pos& operator-=(const int other) {
            this->vertical += other;
            this->horizontal += other;
            return *this;
        }
    };

    // TODO: constexpr std array
    static const std::unordered_set<char> asciiLetters {
        '\x31', '\x32', '\x33', '\x34', '\x35', '\x36', '\x37', '\x38', '\x39', '\x41', '\x42',
        '\x43', '\x44', '\x45', '\x46', '\x47', '\x48', '\x49', '\x4A', '\x4B', '\x4C', '\x4D',
        '\x4E', '\x4F', '\x50', '\x51', '\x52', '\x53', '\x54', '\x55', '\x56', '\x57', '\x58',
        '\x59', '\x5A', '\x61', '\x62', '\x63', '\x64', '\x65', '\x66', '\x67', '\x68', '\x69',
        '\x6A', '\x6B', '\x6C', '\x6D', '\x6E', '\x6F', '\x70', '\x71', '\x72', '\x73', '\x74',
        '\x75', '\x76', '\x77', '\x78', '\x79', '\x7A'};

    void disable_raw_mode();
    [[maybe_unused]] const int enable_raw_mode();
    void enter_alt_screen();
    void exit_alt_screen();
    void enable_signals();
    void sigtstp_handler(std::function<void(void)>);
    void sigcont_handler(std::function<void(void)>);
    [[nodiscard]] const rawterm::Key process_keypress();
    [[nodiscard]] rawterm::Key wait_for_input();
    [[nodiscard]] const rawterm::Pos get_term_size();
    [[nodiscard]] std::string set_terminal_title(const std::string&);
    void clear_screen();
    void clear_line();
    void clear_line_from();
    void clear_line_until();

    void clear_screen_until(const Pos&);
    void clear_screen_from(const Pos&);
    void clear_line(const Pos&);
    void clear_line_until(const Pos&);
    void clear_line_from(const Pos&);
    [[nodiscard]] const std::string to_string(const Mod&);
    [[nodiscard]] int stripped_length(const std::string& txt);

}  // namespace rawterm

#endif  // RAWTERM_CORE_H
