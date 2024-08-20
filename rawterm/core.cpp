#include "core.h"

#include <algorithm>
#include <cstdlib>

#include "cursor.h"
#include "exceptions.h"

namespace rawterm {
    namespace detail {
        [[nodiscard]] bool is_debug() {
            auto raw_env_var = std::getenv("RAWTERM_DEBUG");
            if (raw_env_var == nullptr) {
                return false;
            }

            const std::array<std::string, 3> values = {"1", "true", "TRUE"};
            return std::find(values.begin(), values.end(), raw_env_var) != values.end();
        }

    }  // namespace detail
    rawterm::Mod Key::getMod() {
        if (mod.empty()) {
            return rawterm::Mod::None;
        } else {
            rawterm::Mod val = mod[0];
            mod.pop_front();
            return val;
        }
    }

    bool Key::isCharInput() {
        return std::isprint(static_cast<unsigned char>(code)) && code != ' ' &&
               (mod.empty() || mod[0] == Mod::Shift);
    }

    void disable_raw_mode() {
#if __linux__
        if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &rawterm::detail::orig) == -1) {
            Cursor c;
            c.reset();
            clear_screen();
            c.reset();
            std::perror("tcsetattr");
        }
#elif _WIN32
        SetConsoleOutputCP(437);
#endif
    }

    [[maybe_unused]] int enable_raw_mode() {
#if __linux__
        if (tcgetattr(STDIN_FILENO, &rawterm::detail::orig) == -1) {
            std::perror("tcgetattr");
        }
        std::atexit(rawterm::disable_raw_mode);

        termios raw = rawterm::detail::orig;
        raw.c_iflag &= ~(BRKINT | ICRNL | INPCK | ISTRIP | IXON);
        raw.c_oflag &= ~(OPOST);
        // raw.c_lflag |= ~(CS8); // Disable to allow term_size reading
        raw.c_lflag &= ~(ECHO | IEXTEN | ICANON | ISIG);

        if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw) == -1) {
            std::perror("tcsetattr");
        }
        return 0;
#elif _WIN32
        // https://learn.microsoft.com/en-us/windows/console/console-virtual-terminal-sequences#example-of-sgr-terminal-sequences
        DWORD in_flags = 0;
        if (!GetConsoleMode(GetStdHandle(STD_INPUT_HANDLE), &in_flags)) {
            return GetLastError();
        }

        in_flags &= ~(ENABLE_LINE_INPUT | ENABLE_PROCESSED_INPUT | ENABLE_ECHO_INPUT);
        in_flags |= ENABLE_VIRTUAL_TERMINAL_INPUT;

        SetConsoleMode(GetStdHandle(STD_INPUT_HANDLE), in_flags);
        SetConsoleOutputCP(65001);  // enable utf-8
        std::atexit(rawterm::disable_raw_mode);
        return 0;
#endif
    }

    void enter_alt_screen() {
        if (detail::is_debug()) {
            return;
        }
        std::atexit(rawterm::exit_alt_screen);
        std::cout << "\x1B[?1049h" << std::flush;
    }

    void exit_alt_screen() {
        if (detail::is_debug()) {
            return;
        }
        std::cout << "\x1B[?1049l" << std::flush;
    }

    bool is_signals_enabled = false;
    void enable_signals() {
#if __linux__
        is_signals_enabled = true;
#endif
    }

    void sigtstp_handler(std::function<void(void)> func) {
        if (detail::sigtstp_called) {
            func();
            detail::sigtstp_called = false;
        }
    }

    void sigcont_handler(std::function<void(void)> func) {
        if (detail::sigcont_called) {
            func();
            detail::sigcont_called = false;
        }
    }

    [[nodiscard]] const std::optional<rawterm::Key> process_keypress() {
#if __linux__
        // Backgrounding
        std::signal(SIGTSTP, [](int) {
            detail::sigtstp_called = true;
            rawterm::exit_alt_screen();
            std::raise(SIGSTOP);
        });

        // Foregrounding
        std::signal(SIGCONT, [](int) {
            detail::sigcont_called = true;
            rawterm::enter_alt_screen();
            rawterm::enable_raw_mode();
        });
#endif

        std::string characters = std::string(32, '\0');
        int pollResult = poll(&detail::fd, 1, 0);

        // input available
        if (pollResult > 0) {
            if (read(STDIN_FILENO, characters.data(), 32) < 0) {
                throw rawterm::KeypressError("An error occured during reading user input");
            }

            // no input found
        } else if (pollResult == 0) {
            return {};

            // Error
        } else {
            throw rawterm::KeypressError("An unknown error occured");
        }

        std::stringstream ss;
        ss << std::hex;
        for (char c : characters.substr(0, characters.find('\0'))) {
            ss << "\\x" << static_cast<int>(static_cast<unsigned char>(c));
        }
        const std::string raw = ss.str();

        // TODO: alt-gr, multiple modifier keys?
        // NOTE: https://www.rapidtables.com/code/text/ascii-table.html
        switch (characters[0]) {
            case '\x01':
                return Key('a', rawterm::Mod::Control, raw);
            case '\x02':
                return Key('b', rawterm::Mod::Control, raw);
            case '\x03':
                if (is_signals_enabled) {
                    raise(SIGINT);
                }
                return Key('c', rawterm::Mod::Control, raw);
            case '\x04':
                return Key('d', rawterm::Mod::Control, raw);
            case '\x05':
                return Key('e', rawterm::Mod::Control, raw);
            case '\x06':
                return Key('f', rawterm::Mod::Control, raw);
            case '\x07':
                return Key('g', rawterm::Mod::Control, raw);
            case '\x08':
                return Key('h', rawterm::Mod::Control, raw);
            case '\x09':
                return Key('i', rawterm::Mod::Tab, raw);
            case '\x0A':
                return Key('j', rawterm::Mod::Control, raw);
            case '\x0B':
                return Key('k', rawterm::Mod::Control, raw);
            case '\x0C':
                return Key('l', rawterm::Mod::Control, raw);
            case '\x0D':
                // Enter is ^m
                return Key('m', rawterm::Mod::Enter, raw);
            case '\x0E':
                return Key('n', rawterm::Mod::Control, raw);
            case '\x0F':
                return Key('o', rawterm::Mod::Control, raw);
            case '\x10':
                return Key('p', rawterm::Mod::Control, raw);
            case '\x11':
                return Key('q', rawterm::Mod::Control, raw);
            case '\x12':
                return Key('r', rawterm::Mod::Control, raw);
            case '\x13':
                return Key('s', rawterm::Mod::Control, raw);
            case '\x14':
                return Key('t', rawterm::Mod::Control, raw);
            case '\x15':
                return Key('u', rawterm::Mod::Control, raw);
            case '\x16':
                return Key('v', rawterm::Mod::Control, raw);
            case '\x17':
                return Key('w', rawterm::Mod::Control, raw);
            case '\x18':
                return Key('x', rawterm::Mod::Control, raw);
            case '\x19':
                return Key('y', rawterm::Mod::Control, raw);
            case '\x1A':
#if __linux__
                if (is_signals_enabled) {
                    raise(SIGTSTP);
                }
#endif
                return Key('z', rawterm::Mod::Control, raw);
            case '\x1B':
                // ESCAPE

                // characters[0] is escape char
                // if characters[1] is equal to a letter (upper or lowercase), it's
                // left alt+letter if characters[1] is [, it's an arrow key (abcd
                // after for udrl) if characters[2] is \x31 (1), it can be f5+ f1
                // \x1B\x4F\x50 f2 \x1B\x4F\x51 f3 \x1B\x4F\x52 f4 \x1B\x4F\x53 f5
                // \x1B\x5B\x31\x35\x7E f6 \x1B\x5B\x31\x37\x7E f7
                // \x1B\x5B\x31\x38\x7E f8 \x1B\x5B\x31\x39\x7E f9
                // \x1B\x5B\x32\x30\x7E f10 \x1B\x5B\x32\x31\x7E f11
                // \x1B\x5B\x32\x33\x7E f12 \x1B\x5B\x32\x34\x7E delete:
                // \x1B\x5B\x33\x7E

                if (raw.size() == 4) {
                    return Key(' ', rawterm::Mod::Escape, raw);  // esc
                }

                if (raw.size() == 8 && asciiLetters.contains(characters[1])) {
                    Key k = Key(characters[1], rawterm::Mod::Alt_L, raw);

                    if (characters[1] >= 'A' && characters[1] <= 'Z') {
                        k.mod.push_back(rawterm::Mod::Shift);
                    }

                    return k;
                }
                if (characters[1] == '\x5B') {
                    // ARROWS
                    switch (characters[2]) {
                        case '\x41':
                            return Key('A', rawterm::Mod::Arrow, raw);  // up
                        case '\x42':
                            return Key('B', rawterm::Mod::Arrow, raw);  // down
                        case '\x43':
                            return Key('C', rawterm::Mod::Arrow, raw);  // right
                        case '\x44':
                            return Key('D', rawterm::Mod::Arrow, raw);  // left
                        case '\x33':
                            return Key(' ', rawterm::Mod::Delete, raw);  // delete

                        // FUNCTIONS pt 2
                        case '\x31':
                            switch (characters[3]) {
                                case '\x35':
                                    return Key('5', rawterm::Mod::Function, raw);  // f5
                                case '\x37':
                                    return Key('6', rawterm::Mod::Function, raw);  // f6
                                case '\x38':
                                    return Key('7', rawterm::Mod::Function, raw);  // f7
                                case '\x39':
                                    return Key('8', rawterm::Mod::Function, raw);  // f8
                            }
                            break;

                        case '\x32':
                            switch (characters[3]) {
                                case '\x30':
                                    return Key('9', rawterm::Mod::Function, raw);  // f9
                                case '\x31':
                                    return Key('0', rawterm::Mod::Function, raw);  // f10
                                case '\x33':
                                    return Key('1', rawterm::Mod::Function, raw);  // f11
                                case '\x34':
                                    return Key('2', rawterm::Mod::Function, raw);  // f12
                            }
                            break;
                    }
                } else if (raw.size() == 12 && characters[1] == '\x4F') {
                    // FUNCTIONS pt 1
                    switch (characters[2]) {
                        case '\x50':
                            return Key('1', rawterm::Mod::Function, raw);  // f1
                        case '\x51':
                            return Key('2', rawterm::Mod::Function, raw);  // f2
                        case '\x52':
                            return Key('3', rawterm::Mod::Function, raw);  // f3
                        case '\x53':
                            return Key('4', rawterm::Mod::Function, raw);  // f4
                    }
                }
                break;

            case '\x1C':
                return Key(' ', rawterm::Mod::None, raw);
            case '\x1D':
                return Key(' ', rawterm::Mod::None, raw);
            case '\x1E':
                return Key(' ', rawterm::Mod::None, raw);
            case '\x1F':
                return Key(' ', rawterm::Mod::None, raw);
            case '\x20':
                return Key(' ', rawterm::Mod::Space, raw);
            case '\x21':
                return Key('!', rawterm::Mod::None, raw);
            case '\x22':
                return Key('"', rawterm::Mod::None, raw);
            case '\x23':
                return Key('#', rawterm::Mod::None, raw);
            case '\x24':
                return Key('$', rawterm::Mod::None, raw);
            case '\x25':
                return Key('%', rawterm::Mod::None, raw);
            case '\x26':
                return Key('&', rawterm::Mod::None, raw);
            case '\x27':
                return Key('\'', rawterm::Mod::None, raw);
            case '\x28':
                return Key('(', rawterm::Mod::None, raw);
            case '\x29':
                return Key(')', rawterm::Mod::None, raw);
            case '\x2A':
                return Key('*', rawterm::Mod::None, raw);
            case '\x2B':
                return Key('+', rawterm::Mod::None, raw);
            case '\x2C':
                return Key(',', rawterm::Mod::None, raw);
            case '\x2D':
                return Key('-', rawterm::Mod::None, raw);
            case '\x2E':
                return Key('.', rawterm::Mod::None, raw);
            case '\x2F':
                return Key('/', rawterm::Mod::None, raw);
            case '\x30':
                // NUMBERS
                return Key('0', rawterm::Mod::None, raw);
            case '\x31':
                return Key('1', rawterm::Mod::None, raw);
            case '\x32':
                return Key('2', rawterm::Mod::None, raw);
            case '\x33':
                return Key('3', rawterm::Mod::None, raw);
            case '\x34':
                return Key('4', rawterm::Mod::None, raw);
            case '\x35':
                return Key('5', rawterm::Mod::None, raw);
            case '\x36':
                return Key('6', rawterm::Mod::None, raw);
            case '\x37':
                return Key('7', rawterm::Mod::None, raw);
            case '\x38':
                return Key('8', rawterm::Mod::None, raw);
            case '\x39':
                return Key('9', rawterm::Mod::None, raw);
            case '\x3A':
                return Key(':', rawterm::Mod::None, raw);
            case '\x3B':
                return Key(';', rawterm::Mod::None, raw);
            case '\x3C':
                return Key('<', rawterm::Mod::None, raw);
            case '\x3D':
                return Key('=', rawterm::Mod::None, raw);
            case '\x3E':
                return Key('>', rawterm::Mod::None, raw);
            case '\x3F':
                return Key('?', rawterm::Mod::None, raw);
            case '\x40':
                return Key('@', rawterm::Mod::None, raw);
            case '\x41':
                // UPPERCASE LETTERS
                return Key('A', rawterm::Mod::Shift, raw);
            case '\x42':
                return Key('B', rawterm::Mod::Shift, raw);
            case '\x43':
                return Key('C', rawterm::Mod::Shift, raw);
            case '\x44':
                return Key('D', rawterm::Mod::Shift, raw);
            case '\x45':
                return Key('E', rawterm::Mod::Shift, raw);
            case '\x46':
                return Key('F', rawterm::Mod::Shift, raw);
            case '\x47':
                return Key('G', rawterm::Mod::Shift, raw);
            case '\x48':
                return Key('H', rawterm::Mod::Shift, raw);
            case '\x49':
                return Key('I', rawterm::Mod::Shift, raw);
            case '\x4A':
                return Key('J', rawterm::Mod::Shift, raw);
            case '\x4B':
                return Key('K', rawterm::Mod::Shift, raw);
            case '\x4C':
                return Key('L', rawterm::Mod::Shift, raw);
            case '\x4D':
                return Key('M', rawterm::Mod::Shift, raw);
            case '\x4E':
                return Key('N', rawterm::Mod::Shift, raw);
            case '\x4F':
                return Key('O', rawterm::Mod::Shift, raw);
            case '\x50':
                return Key('P', rawterm::Mod::Shift, raw);
            case '\x51':
                return Key('Q', rawterm::Mod::Shift, raw);
            case '\x52':
                return Key('R', rawterm::Mod::Shift, raw);
            case '\x53':
                return Key('S', rawterm::Mod::Shift, raw);
            case '\x54':
                return Key('T', rawterm::Mod::Shift, raw);
            case '\x55':
                return Key('U', rawterm::Mod::Shift, raw);
            case '\x56':
                return Key('V', rawterm::Mod::Shift, raw);
            case '\x57':
                return Key('W', rawterm::Mod::Shift, raw);
            case '\x58':
                return Key('X', rawterm::Mod::Shift, raw);
            case '\x59':
                return Key('Y', rawterm::Mod::Shift, raw);
            case '\x5A':
                return Key('Z', rawterm::Mod::Shift, raw);
            case '\x5B':
                return Key('[', rawterm::Mod::None, raw);
            case '\x5C':
                return Key('\\', rawterm::Mod::None, raw);
            case '\x5D':
                return Key(']', rawterm::Mod::None, raw);
            case '\x5E':
                return Key('^', rawterm::Mod::None, raw);
            case '\x5F':
                return Key('_', rawterm::Mod::None, raw);
            case '\x60':
                return Key('`', rawterm::Mod::None, raw);
            case '\x61':
                // LOWERCASE LETTERS
                return Key('a', rawterm::Mod::None, raw);
            case '\x62':
                return Key('b', rawterm::Mod::None, raw);
            case '\x63':
                return Key('c', rawterm::Mod::None, raw);
            case '\x64':
                return Key('d', rawterm::Mod::None, raw);
            case '\x65':
                return Key('e', rawterm::Mod::None, raw);
            case '\x66':
                return Key('f', rawterm::Mod::None, raw);
            case '\x67':
                return Key('g', rawterm::Mod::None, raw);
            case '\x68':
                return Key('h', rawterm::Mod::None, raw);
            case '\x69':
                return Key('i', rawterm::Mod::None, raw);
            case '\x6A':
                return Key('j', rawterm::Mod::None, raw);
            case '\x6B':
                return Key('k', rawterm::Mod::None, raw);
            case '\x6C':
                return Key('l', rawterm::Mod::None, raw);
            case '\x6D':
                return Key('m', rawterm::Mod::None, raw);
            case '\x6E':
                return Key('n', rawterm::Mod::None, raw);
            case '\x6F':
                return Key('o', rawterm::Mod::None, raw);
            case '\x70':
                return Key('p', rawterm::Mod::None, raw);
            case '\x71':
                return Key('q', rawterm::Mod::None, raw);
            case '\x72':
                return Key('r', rawterm::Mod::None, raw);
            case '\x73':
                return Key('s', rawterm::Mod::None, raw);
            case '\x74':
                return Key('t', rawterm::Mod::None, raw);
            case '\x75':
                return Key('u', rawterm::Mod::None, raw);
            case '\x76':
                return Key('v', rawterm::Mod::None, raw);
            case '\x77':
                return Key('w', rawterm::Mod::None, raw);
            case '\x78':
                return Key('x', rawterm::Mod::None, raw);
            case '\x79':
                return Key('y', rawterm::Mod::None, raw);
            case '\x7A':
                return Key('z', rawterm::Mod::None, raw);
            case '\x7B':
                return Key('{', rawterm::Mod::None, raw);
            case '\x7C':
                return Key('|', rawterm::Mod::None, raw);
            case '\x7D':
                return Key('}', rawterm::Mod::None, raw);
            case '\x7E':
                return Key('~', rawterm::Mod::None, raw);
            case '\x7F':
                // BACKSPACE
                return Key(' ', rawterm::Mod::Backspace, raw);
        }
        return Key(' ', rawterm::Mod::Unknown, raw);
    }

    const rawterm::Key wait_for_input() {
        while (true) {
            auto k = process_keypress();
            if (k.has_value()) {
                return k.value();
            }
        }
    }

    [[nodiscard]] const rawterm::Pos get_term_size() {
#if __linux__
        struct winsize w;
        ioctl(0, TIOCGWINSZ, &w);
        return Pos {w.ws_row, w.ws_col};

#elif _WIN32
        CONSOLE_SCREEN_BUFFER_INFO csbi;
        GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
        unsigned int columns = csbi.srWindow.Right - csbi.srWindow.Left + 1;
        unsigned int rows = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;

        return Pos {rows, columns};
#endif
    }

    [[nodiscard]] std::string set_terminal_title(const std::string& title) {
        return "\u001b]0;" + title + '\a';
    }

    void clear_screen() {
        if (detail::is_debug()) {
            return;
        }
        std::cout << "\x1B[2J\x1B[H";
    }

    void clear_line() {
        if (detail::is_debug()) {
            return;
        }
        std::cout << "\x1B[2K\r";
    }

    void clear_line_from() {
        if (detail::is_debug()) {
            return;
        }
        std::cout << "\x1B[0K";
    }

    void clear_line_until() {
        if (detail::is_debug()) {
            return;
        }
        std::cout << "\x1B[1K";
    }

    void clear_screen_until(const Pos& pos) {
        if (detail::is_debug()) {
            return;
        }
        std::cout << "\x1B[s\x1B[" << std::to_string(pos.vertical) << ';'
                  << std::to_string(pos.horizontal) << "H\x1B[1J\x1B[u";
    }

    void clear_screen_from(const Pos& pos) {
        if (detail::is_debug()) {
            return;
        }
        std::cout << "\x1B[s\x1B[" << std::to_string(pos.vertical) << ';'
                  << std::to_string(pos.horizontal) << "H\x1B[0J\x1B[u";
    }

    void clear_line(const Pos& pos) {
        if (detail::is_debug()) {
            return;
        }
        std::cout << "\x1B[s\x1B[" << std::to_string(pos.vertical) << "H\x1B[2K\x1B[u";
    }

    void clear_line_until(const Pos& pos) {
        if (detail::is_debug()) {
            return;
        }
        std::cout << "\x1B[s\x1B[" << std::to_string(pos.vertical) << ';'
                  << std::to_string(pos.horizontal) << "H\x1B[1K\x1B[u";
    }

    void clear_line_from(const Pos& pos) {
        if (detail::is_debug()) {
            return;
        }
        std::cout << "\x1B[s\x1B[" << std::to_string(pos.vertical) << ';'
                  << std::to_string(pos.horizontal) << "H\x1B[0K\x1B[u";
    }

    [[nodiscard]] const std::string to_string(const Mod& m) {
        switch (m) {
            case Mod::Alt_L:
                return "Alt_L";
            case Mod::Arrow:
                return "Arrow";
            case Mod::Backspace:
                return "Backspace";
            case Mod::Control:
                return "Control";
            case Mod::Delete:
                return "Delete";
            case Mod::Enter:
                return "Enter";
            case Mod::Escape:
                return "Escape";
            case Mod::Function:
                return "Function";
            case Mod::None:
                return "None";
            case Mod::Shift:
                return "Shift";
            case Mod::Space:
                return "Space";
            case Mod::Tab:
                return "Tab";
            case Mod::Unknown:
                return "Unknown";
        }

        return "";
    }
}  // namespace rawterm
