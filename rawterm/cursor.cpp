#include "cursor.h"

namespace rawterm {
    void Cursor::reset() {
        std::cout << "\x1B[" << std::to_string(vertical) << ';'
                  << std::to_string(horizontal) << 'H' << std::flush;
    }

    // TODO: Bounds checking on these
    void Cursor::move(const Pos& pos) {
        // These save the given position to the cursor object
        vertical = pos.vertical;
        horizontal = pos.vertical;
        
        std::cout << "\x1B[" << std::to_string(pos.vertical) << ';'
                  << std::to_string(pos.horizontal) << 'H' << std::flush;
    }

    void Cursor::move_up(int in = 1) {
        vertical--;
        std::cout << "\x1b[" + std::to_string(in) + "A" << std::flush; 
    }

    void Cursor::move_down(int in = 1) {
        vertical++;
        std::cout << "\x1b[" + std::to_string(in) + "B" << std::flush; 
    }

    void Cursor::move_right(int in = 1) {
        horizontal++;
        std::cout << "\x1b[" + std::to_string(in) + "C" << std::flush; 
    }

    void Cursor::move_left(int in = 1) {
        horizontal--;
        std::cout << "\x1b[" + std::to_string(in) + "D" << std::flush; 
    }

    void Cursor::offset_cursor(Pos offset) {
        if (offset.horizontal < 0) {
            std::cout << "\x1B[" << -offset.horizontal << 'D';
        } else if (offset.horizontal > 0) {
            std::cout << "\x1B[" << offset.horizontal << 'C';
        }

        if (offset.vertical < 0) {
            std::cout << "\x1B[" << -offset.vertical << 'A';
        } else if (offset.vertical > 0) {
            std::cout << "\x1B[" << offset.vertical << 'B';
        }
    }

    void Cursor::save_cursor_position() { std::cout << "\x1B[s" << std::flush; }

    void Cursor::load_cursor_position() { std::cout << "\x1B[u" << std::flush; }

    std::optional<Pos> Cursor::get_raw_location() {
#if __linux__
        std::cout << "\x1B[6n" << std::flush;

        std::string response;
        char input;
        while (std::cin.get(input)) {
            if (input == 'R') {
                break;
            }
            response += input;
        }

        std::size_t found = response.find(';');
        if (found != std::string::npos) {
            std::string row_str = response.substr(2, found - 2);
            std::string col_str = response.substr(found + 1);

            return Pos{std::stoi(row_str), std::stoi(col_str)};
        }
        
#elif _WIN32
        // TODO: Test windows cursor location code
        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        CONSOLE_SCREEN_BUFFER_INFO csbi;
        COORD cursorPosition;

        if (GetConsoleScreenBufferInfo(hConsole, &csbi)) {
            cursorPosition = csbi.dwCursorPosition;
            return Pos{cursorPosition.Y + 1, cursorPosition.X + 1};
        }
#endif

        return {};
    }

    void Cursor::cursor_block_blink() { std::cout << "\1\x1B[1 q\2" << std::flush; }

    void Cursor::cursor_block() { std::cout << "\1\x1B[2 q\2" << std::flush; }

    void Cursor::cursor_underscore_blink() { std::cout << "\1\x1B[3 q\2" << std::flush; }

    void Cursor::cursor_underscore() { std::cout << "\1\x1B[4 q\2" << std::flush; }

    void Cursor::cursor_pipe_blink() { std::cout << "\1\x1B[5 q\2" << std::flush; }

    void Cursor::cursor_pipe() { std::cout << "\1\x1B[6 q\6" << std::flush; }

    void Cursor::cursor_hide() { std::cout << "\x1B[?25l" << std::flush; }

    void Cursor::cursor_show() { std::cout << "\x1B[?25h" << std::flush; }

}
