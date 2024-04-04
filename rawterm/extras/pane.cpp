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
        for (int y = 0; y < dimensions.vertical; y++) {
            for (int x = 0; x < dimensions.horizontal; x++) {
                std::cout << " ";
            }
                std::cout << "\r\n";
        }
    }


    template<typename T>
    [[nodiscard]] std::unique_ptr<Pane<T>> Pane<T>::split_vertical(std::span<T> new_content) {
        Pos new_dims = {dimensions.vertical, dimensions.horizontal / 2} ;
        if (dimensions.horizontal % 2) { new_dims.horizontal + 1; }

        Pos split_start = {new_dims.vertical, new_dims.horizontal + 1};
        dimensions = new_dims;
        
        draw();
        return Pane<T>::make(split_start, new_dims, new_content);
    }

    template<typename T>
    [[nodiscard]] std::unique_ptr<Pane<T>> Pane<T>::split_horizontal(std::span<T> new_content) {
        Pos new_dims = {dimensions.horizontal, dimensions.vertical / 2} ;
        if (dimensions.vertical % 2) { new_dims.vertical + 1; }

        Pos split_start = {new_dims.horizontal, new_dims.vertical + 1};
        dimensions = new_dims;
        
        draw();
        // return Pane<T>::make(split_start, new_dims, std::span<T>());
        return Pane<T>::make(split_start, new_dims, new_content);

    }

    template<typename T>
    void Pane<T>::resize(const Pos& new_size) {
        dimensions = new_size;
        draw();
    }

}
