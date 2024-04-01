#include <iostream>

#include "pane.h"

namespace rawterm {

    template<typename T>
    std::unique_ptr<Pane<T>> Pane<T>::make(
        const Pos& start,
        const Pos& dimensions,
        std::span<T> content) {
            return std::make_unique<Pane>(start, dimensions, content);
    }

    template<typename T>
    void Pane<T>::clear() {
        cur.move(start_point);
        Pos end = start_point + dimensions;
        rawterm::clear_screen_until(end);
    }

    template<typename T>
    void Pane<T>::draw() {
        clear();
        cur.move(start_point);

        for (auto&& line: content) {
            if (line.size() < dimensions.horizontal) {
                std::cout << line << "\r\n";
            } else if (line.size() >= dimensions.horizontal) {
                std::cout << line.substr(0, dimensions.horizontal - 1) << "\r\n";
            }
        }

        cur.move(start_point);
    }

    template<typename T>
    void Pane<T>::set_pane_background(const Color& color) {
        cur.move(start_point);

        std::cout << color;
        for (auto&& y: dimensions.vertical) {
            for (auto&& x: dimensions.horizontal) {
                std::cout << " ";
            }
                std::cout << "\r\n";
        }
    }

}
