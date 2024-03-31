#ifndef RAWTERM_CURSOR_H
#define RAWTERM_CURSOR_H

#include "core.h"

namespace rawterm {
    struct Cursor : Pos {
        Cursor() : Pos{1, 1} {}
        void reset();
        void move(rawterm::Pos);
        void move_up(int); 
        void move_down(int);
        void move_right(int);
        void move_left(int);
        void offset_cursor(Pos);

        static void save_cursor_position();
        static void load_cursor_position();
        static void cursor_block_blink();
        static void cursor_block();
        static void cursor_underscore_blink();
        static void cursor_underscore();
        static void cursor_pipe_blink();
        static void cursor_pipe();
        static void cursor_hide();
        static void cursor_show();
    };
}                                            

#endif // RAWTERM_CURSOR_H
