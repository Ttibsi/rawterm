#ifndef RAWTERM_EXTRAS_PANE_H
#define RAWTERM_EXTRAS_PANE_H

#include <memory>
#include <span>

#include <rawterm/color.h>
#include <rawterm/core.h>
#include <rawterm/cursor.h>

namespace rawterm {
    template<typename T>
    class Pane {
        private:
            Pos start_point;
            Pos dimensions;
            std::span<T> content;
            Cursor cur;

            Pane(Pos s, Pos d, std::span<T> c): 
                start_point(s), dimensions(d), content(c), cur(Cursor(s)) {}
        public:
            static std::unique_ptr<Pane> make();
            void clear();
            void draw();
            void set_pane_background(const Color&);

            // TODO: resizing panes
            void split_vertical();
            void split_horizontal();
            void resize();
    };

}

#endif //RAWTERM_EXTRAS_PANE_H
