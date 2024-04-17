#ifndef RAWTERM_EXTRAS_PANE_H
#define RAWTERM_EXTRAS_PANE_H

#include <iostream>
#include <memory>
#include <vector>

#include <rawterm/color.h>
#include <rawterm/core.h>
#include <rawterm/cursor.h>

namespace rawterm {
    template<typename T = std::vector<std::string> >
    class Pane {
        private:
            const Pos start_point;
            Pos dimensions;
            T content;
            Cursor cur;
            std::optional<Color> background_color;

        public:
            Pane(const Pos& s, Pos d, T c): 
                start_point(s), dimensions(d), content(c), cur(Cursor(s)) {}

            [[nodiscard]] static std::unique_ptr<Pane<T>> make(
                const Pos& start,
                const Pos& dimensions,
                const T content) {
                    return std::make_unique<Pane>(start, dimensions, content);
            }

            void clear() {
                Pos end = start_point + dimensions;

                for (int i = start_point.vertical; i < end.vertical; i++) {
                    for (int j = start_point.horizontal; j < end.horizontal; j++) {
                        cur.move({i, j});
                        
                        if (background_color.has_value()) {
                            std::cout << "\x1B[48;2;" << background_color.value() << " " << "\x1B[49m";
                        } else {
                            std::cout << " ";

                        }
                    }
                }
            }

            void draw() {
                clear();
                cur.move(start_point);

                for (int idx = 0; idx < content.size(); idx++) {
                    if (background_color.has_value()) {
                        std::cout << "\x1B[48;2;" << background_color.value();
                    }
                    std::cout << content[idx];
                    cur.move({start_point.vertical + idx + 1, start_point.horizontal});
                }

                cur.move(start_point);
            }

            void set_pane_background(const Color& col) { background_color = col; }


            [[nodiscard]] std::unique_ptr<Pane<T>> split_vertical(T new_content) {
                Pos new_dims = {dimensions.vertical, dimensions.horizontal / 2} ;
                if (dimensions.horizontal % 2) { new_dims.horizontal + 1; }

                Pos split_start = {start_point.vertical, new_dims.horizontal + 1};
                dimensions = new_dims;
                
                return make(split_start, new_dims, new_content);
            }

            [[nodiscard]] std::unique_ptr<Pane<T>> split_vertical() {
                return split_vertical(T());
            }

            [[nodiscard]] std::unique_ptr<Pane<T>> split_horizontal(T new_content) {
                Pos new_dims = {dimensions.vertical / 2, dimensions.horizontal} ;
                bool extra_row = false; 
                if (dimensions.vertical % 2 == 1) { extra_row = true; }
                if (extra_row) { new_dims.vertical + 1; }

                Pos split_start = {new_dims.vertical + 1, start_point.horizontal};
                dimensions = new_dims;
                
                if (extra_row) { new_dims.vertical - 1; }
                return make(split_start, new_dims, new_content);
            }

            [[nodiscard]] std::unique_ptr<Pane<T>> split_horizontal() {
                return split_horizontal(T());
            }

            void resize(const Pos& new_size) {
                dimensions = new_size;
                draw();
            }
    };

    template<typename T = std::vector<std::string>>
    using pane_t = std::unique_ptr<Pane<T>>;
}

#endif //RAWTERM_EXTRAS_PANE_H
