#ifndef RAWTERM_EXTRAS_PANE_H
#define RAWTERM_EXTRAS_PANE_H

#include <iostream>
#include <memory>
#include <span>

#include <rawterm/color.h>
#include <rawterm/core.h>
#include <rawterm/cursor.h>

namespace rawterm {
    template<typename T = std::span<std::string> >
    class Pane {
        private:
            const Pos start_point;
            Pos dimensions;
            const T content;
            Cursor cur;

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
                        cur.move({i - 1, j - 1});
                        std::cout << " ";
                    }
                }
            }

            void draw() {
                clear();
                cur.move(start_point);

                for (int idx = 0; idx < content.size(); idx++) {
                    std::cout << content[idx] << content[idx].size();
                    cur.move({start_point.vertical + 1, start_point.horizontal});
                }

                cur.move(start_point);
            }

            void set_pane_background(const Color& color) {
                cur.move(start_point);

                std::cout << color;
                for (int y = 0; y < dimensions.vertical; y++) {
                    for (int x = 0; x < dimensions.horizontal; x++) {
                        std::cout << " ";
                    }
                        std::cout << "\r\n";
                }
            }


            // TODO: Empty split
            [[nodiscard]] std::unique_ptr<Pane<T>> split_vertical(T new_content) {
                Pos new_dims = {dimensions.vertical, dimensions.horizontal / 2} ;
                if (dimensions.horizontal % 2) { new_dims.horizontal + 1; }

                Pos split_start = {start_point.vertical, new_dims.horizontal + 1};
                dimensions = new_dims;
                
                draw();
                return make(split_start, new_dims, new_content);
            }

            [[nodiscard]] std::unique_ptr<Pane<T>> split_horizontal(T new_content) {
                Pos new_dims = {dimensions.horizontal, dimensions.vertical / 2} ;
                if (dimensions.vertical % 2) { new_dims.vertical + 1; }

                Pos split_start = {new_dims.vertical + 1, start_point.horizontal};
                dimensions = new_dims;
                
                draw();
                // return make(split_start, new_dims, std::span<T>());
                return make(split_start, new_dims, new_content);

            }

            void resize(const Pos& new_size) {
                dimensions = new_size;
                draw();
            }
    };

    template<typename T = std::span<std::string>>
    using pane_t = std::unique_ptr<Pane<T>>;
}

#endif //RAWTERM_EXTRAS_PANE_H
