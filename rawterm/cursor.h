#ifndef RAWTERM_CURSOR_H
#define RAWTERM_CURSOR_H

#include <optional>

#include "screen.h"

namespace rawterm {
    struct Cursor : Pos {
        Cursor() : Pos {1, 1} {}
        Cursor(Pos p) : Pos {p} {}
        void reset() const;
        void move(const Pos&);
        void move(const int, const int);
        void move_up(int = 1);
        void move_down(int = 1);
        void move_right(int = 1);
        void move_left(int = 1);
        void offset_cursor(Pos);

        static void save_cursor_position();
        static void load_cursor_position();
        static const std::optional<Pos> get_raw_location();
        static void cursor_block_blink();
        static void cursor_block();
        static void cursor_underscore_blink();
        static void cursor_underscore();
        static void cursor_pipe_blink();
        static void cursor_pipe();
        static void cursor_hide();
        static void cursor_show();
    };
}  // namespace rawterm

#endif  // RAWTERM_CURSOR_H
