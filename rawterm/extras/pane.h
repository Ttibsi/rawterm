#ifndef RAWTERM_PANE_H
#define RAWTERM_PANE_H

// This pragma block sets this header to be a system header, disabling any
// warnings from being printed when compiling
// I don't want to fix the new warnings on this file as I intend one day
// to come back and rewrite this as it's experimental
#ifdef __clang__
#pragma clang system_header
#else
#pragma GCC system_header
#endif

#include <algorithm>
#include <array>
#include <functional>
#include <memory>
#include <optional>
#include <stdexcept>
#include <string>
#include <string_view>
#include <type_traits>
#include <vector>

#include <rawterm/color.h>
#include <rawterm/core.h>
#include <rawterm/cursor.h>
#include <rawterm/screen.h>
#include <rawterm/text.h>

namespace rawterm {
    template <typename T = std::vector<std::string>>
    class PaneManager {
       private:
        struct Pane {
            unsigned int id;
            Pos origin;
            Pos dimensions;
            Cursor cur;
            T content;
            std::optional<Color> background_color;
            std::vector<Region> cursor_blacklist_regions;

            Pane* parent = nullptr;
            Pane* left = nullptr;
            Pane* right = nullptr;

            // Initial ctor
            Pane(const Pos& term_dimensions)
                : id(1),
                  origin({1, 1}),
                  dimensions(term_dimensions),
                  cur(Cursor()),
                  content(T()),
                  cursor_blacklist_regions({}) {
                cur.reset();
            }

            // Splitting ctor
            Pane(
                int new_id,
                Pos new_origin,
                Pos new_dims,
                Cursor new_cursor,
                T c,
                Color color,
                std::vector<Region> regs)
                : id(new_id),
                  origin(new_origin),
                  dimensions(new_dims),
                  cur(new_cursor),
                  content(c),
                  background_color(color),
                  cursor_blacklist_regions(regs) {}

            void clear() {
                if (detail::is_debug()) {
                    return;
                }
                Pos current_pos = cur;
                Pos end = origin + dimensions;

                for (int i = origin.vertical; i < end.vertical; i++) {
                    for (int j = origin.horizontal; j < end.horizontal; j++) {
                        cur.move({i, j});

                        if (background_color.has_value()) {
                            std::cout << "\x1B[48;2;" << background_color.value() << " "
                                      << "\x1B[49m";
                        } else {
                            std::cout << " ";
                        }
                    }
                }
                cur = current_pos;
            }

            void line_printer(std::string& line) {
                if (raw_size(line) > dimensions.horizontal) {
                    line = line.substr(0, dimensions.horizontal);
                }

                if (background_color.has_value()) {
                    std::cout << "\x1B[48;2;" << background_color.value();
                }
                std::cout << line;
            }

            void draw() {
                if (detail::is_debug()) {
                    return;
                }

                clear();
                Pos cur_pos = cur;
                cur.move(origin);

                if constexpr (std::is_same_v<typename T::value_type, char>) {
                    std::string line = "";
                    int line_count = 0;

                    for (int idx = 0; idx < static_cast<int>(content.size()); idx++) {
                        char out = content.at(idx);
                        line.push_back(out);
                        if (out == '\n') {
                            line_count++;
                            if (line_count == dimensions.vertical) {
                                break;
                            }

                            line_printer(line);
                            cur.move(
                                {static_cast<int>(origin.vertical + line_count + 1),
                                 origin.horizontal});

                            line = "";
                        }
                    }
                } else {
                    for (int idx = 0; idx < static_cast<int>(content.size()); idx++) {
                        if (idx >= dimensions.vertical) {
                            break;
                        }

                        typename T::value_type out = content.at(idx);
                        line_printer(out);
                        cur.move({static_cast<int>(origin.vertical + idx + 1), origin.horizontal});
                    }
                }

                cur.move(cur_pos);
                move_cur_out_of_blacklist();
            }

            void move_cur_out_of_blacklist() {
                for (auto&& bl : cursor_blacklist_regions) {
                    while (bl.contains(cur)) {
                        // TODO: Handle going off the side of the screen
                        cur.move_right();
                    }
                }
            }

            void move_cur_up() {
                cur.move_up();

                for (auto&& bl : cursor_blacklist_regions) {
                    while (bl.contains(cur)) {
                        cur.move_up();
                    }
                }

                if (cur.partial_cmp(origin) || cur.partial_cmp(origin + dimensions)) {
                    throw std::out_of_range("Cannot move cursor outside of pane dimensions");
                }
            }

            void move_cur_left() {
                cur.move_left();

                for (auto&& bl : cursor_blacklist_regions) {
                    while (bl.contains(cur)) {
                        cur.move_left();
                    }
                }

                if (cur.partial_cmp(origin) || cur.partial_cmp(origin + dimensions)) {
                    throw std::out_of_range("Cannot move cursor outside of pane dimensions");
                }
            }

            void move_cur_right() {
                cur.move_right();

                for (auto&& bl : cursor_blacklist_regions) {
                    while (bl.contains(cur)) {
                        cur.move_right();
                    }
                }

                if (cur.partial_cmp(origin) || cur.partial_cmp(origin + dimensions)) {
                    throw std::out_of_range("Cannot move cursor outside of pane dimensions");
                }
            }

            void move_cur_down() {
                cur.move_down();

                for (auto&& bl : cursor_blacklist_regions) {
                    while (bl.contains(cur)) {
                        cur.move_down();
                    }
                }

                if (cur.partial_cmp(origin) || cur.partial_cmp(origin + dimensions)) {
                    throw std::out_of_range("Cannot move cursor outside of pane dimensions");
                }
            }

            [[nodiscard]] constexpr bool is_leaf() { return (left == nullptr && right == nullptr); }

            [[nodiscard]] constexpr bool is_left_node() {
                if (parent == nullptr) {
                    return false;
                }
                return parent->left == this;
            }

            [[nodiscard]] constexpr bool is_split_vertical() {
                if (is_leaf()) {
                    return false;
                }
                return left->dimensions.vertical == right->dimensions.vertical;
            }

            [[nodiscard]] constexpr bool is_root() { return parent == nullptr; }

            void kill_children() {
                left = nullptr;
                right = nullptr;
            }

            friend std::ostream& operator<<(std::ostream& os, const Pane p) {
                int parent_id = (p.parent == nullptr ? 0 : p.parent->id);
                return os << "Pane{" << p.id << ", origin{" << p.origin << "}, dims{"
                          << p.dimensions << "}, Parent: " << parent_id << "}";
            }
        };

        std::vector<Pane> pane_bank = {};
        Pane* active_pane;

        void check_all(Pane* p, std::function<void(Pane*)> func) {
            if (p->left != nullptr) {
                check_all(p->left, func);
            }
            if (p->right != nullptr) {
                check_all(p->right, func);
            }
            func(p);
        }

        [[nodiscard]] Pane* get_pane(const int id) {
            auto it = std::find_if(
                pane_bank.begin(), pane_bank.end(), [id](Pane& p) { return p.id == id; });

            return &*it;
        }

        void remove_from_bank(int id) {
            Pane* p = get_pane(id);

            if (!(p->is_root())) {
                p->is_left_node() ? p->parent->left = nullptr : p->parent->right = nullptr;
            }

            pane_bank.erase(
                std::remove_if(
                    pane_bank.begin(), pane_bank.end(), [&id](Pane p) { return p.id == id; }),
                pane_bank.end());
        }

       public:
        PaneManager(const Pos& term_dimensions) {
            pane_bank.reserve(12);
            pane_bank.push_back(Pane(term_dimensions));
            active_pane = &pane_bank.at(0);
        }

        void print_all() {
            for (auto p : pane_bank) {
                std::cout << p << "\n";
            }
            std::cout << "Active pane: " << active_pane->id << "\n";
        }

        [[nodiscard]] unsigned int count() const { return pane_bank.size(); }
        [[nodiscard]] const Pos get_cur_pos() const { return active_pane->cur; }
        [[nodiscard]] const std::string get_cur_pos_str() const { return active_pane->cur.toStr(); }
        [[nodiscard]] unsigned int active() const { return active_pane->id; }
        [[nodiscard]] const Pos get_size() const { return active_pane->dimensions; }

        void set_content(T new_c) {
            active_pane->content = new_c;
            active_pane->draw();
            active_pane->cur.move(active_pane->origin);
            active_pane->cur.reset();
        }

        void set_blacklist_region(Region bl) {
            active_pane->cursor_blacklist_regions.push_back(bl);

            while (bl.contains(active_pane->cur)) {
                active_pane->cur += {0, 1};
            }
        }

        void set_pane_background(const Color& col) {
            active_pane->background_color = col;
            active_pane->draw();
        }

        void resize(const Pos& new_size) { active_pane->dimensions = new_size; }

        void move_cursor_up(int count = 1) {
            for (int idx = 0; idx < count; idx++) {
                active_pane->move_cur_up();
            }
        }

        void move_cursor_left(int count = 1) {
            for (int idx = 0; idx < count; idx++) {
                active_pane->move_cur_left();
            }
        }

        void move_cursor_right(int count = 1) {
            for (int idx = 0; idx < count; idx++) {
                active_pane->move_cur_right();
            }
        }

        void move_cursor_down(int count = 1) {
            for (int idx = 0; idx < count; idx++) {
                active_pane->move_cur_down();
            }
        }

        unsigned int split_vertical(T new_c) {
            Pos half_dims = {
                active_pane->dimensions.vertical, active_pane->dimensions.horizontal / 2};

            bool odd_split = false;
            if (active_pane->dimensions.horizontal % 2) {
                odd_split = true;
            }

            Pos split_origin = {
                active_pane->origin.vertical, half_dims.horizontal + (odd_split ? 2 : 1)};

            std::vector<Region> new_left = {};
            std::vector<Region> new_right = {};

            for (auto&& r : active_pane->cursor_blacklist_regions) {
                if (r.top_left.horizontal < active_pane->dimensions.horizontal / 2) {
                    new_left.emplace_back(
                        r.top_left,
                        Pos(r.bottom_right.vertical, std::min(
                                                         r.bottom_right.horizontal / 2,
                                                         active_pane->dimensions.horizontal)));
                }

                if (r.bottom_right.horizontal < active_pane->dimensions.horizontal / 2) {
                    new_right.emplace_back(
                        Pos(r.top_left.vertical,
                            std::max(
                                r.top_left.horizontal / 2, active_pane->dimensions.horizontal)),
                        r.bottom_right);
                }
            }

            pane_bank.emplace_back(
                pane_bank.size() + 1, active_pane->origin,
                odd_split ? Pos {half_dims.vertical, half_dims.horizontal + 1} : half_dims,
                Cursor({1, active_pane->cur.horizontal}), active_pane->content,
                active_pane->background_color.value(), new_left);

            pane_bank.emplace_back(
                pane_bank.size() + 1, split_origin, half_dims, split_origin, new_c, Color(),
                new_right);

            // Extra indirection needed to ensure we have a pointer to the object,
            // not to a location in the vector
            Pane& p1 = pane_bank[pane_bank.size() - 2];
            Pane& p2 = pane_bank[pane_bank.size() - 1];
            active_pane->left = &p1;
            active_pane->right = &p2;

            Pane& p3 = *active_pane;
            active_pane->left->parent = &p3;
            active_pane->right->parent = &p3;
            active_pane = active_pane->left;

            draw_all();
            return pane_bank.size();
        }

        unsigned int split_horizontal(T new_c) {
            Pos half_dims = {
                active_pane->dimensions.vertical / 2, active_pane->dimensions.horizontal};

            bool odd_split = false;
            if (active_pane->dimensions.vertical % 2) {
                odd_split = true;
            }

            Pos split_origin = {
                half_dims.vertical + (odd_split ? 2 : 1), active_pane->origin.horizontal};

            std::vector<Region> new_left = {};
            std::vector<Region> new_right = {};

            for (auto&& r : active_pane->cursor_blacklist_regions) {
                if (r.top_left.horizontal < active_pane->dimensions.horizontal / 2) {
                    new_left.emplace_back(
                        r.top_left,
                        Pos(r.bottom_right.vertical, std::min(
                                                         r.bottom_right.horizontal / 2,
                                                         active_pane->dimensions.horizontal)));
                }

                if (r.bottom_right.horizontal < active_pane->dimensions.horizontal / 2) {
                    new_right.emplace_back(
                        Pos(r.top_left.vertical,
                            std::max(
                                r.top_left.horizontal / 2, active_pane->dimensions.horizontal)),
                        r.bottom_right);
                }
            }

            pane_bank.emplace_back(
                pane_bank.size() + 1, active_pane->origin,
                odd_split ? Pos {half_dims.vertical + 1, half_dims.horizontal} : half_dims,
                Cursor({active_pane->cur.vertical, 1}), active_pane->content,
                active_pane->background_color.value(), new_left);

            pane_bank.emplace_back(
                pane_bank.size() + 1, split_origin, half_dims, split_origin, new_c, Color(),
                new_right);

            // Extra indirection needed to ensure we have a pointer to the object,
            // not to a location in the vector
            Pane& p1 = pane_bank[pane_bank.size() - 2];
            Pane& p2 = pane_bank[pane_bank.size() - 1];
            active_pane->left = &p1;
            active_pane->right = &p2;

            Pane& p3 = *active_pane;
            active_pane->left->parent = &p3;
            active_pane->right->parent = &p3;
            active_pane = active_pane->left;

            draw_all();
            return pane_bank.size();
        }

        unsigned int split_vertical() { return split_vertical({}); }
        unsigned int split_horizontal() { return split_horizontal({}); }

        void switch_active(unsigned int idx) {
            auto it = std::find_if(pane_bank.begin(), pane_bank.end(), [this](Pane& p) {
                return p.is_leaf() && p.id > active_pane->id;
            });

            if (it == pane_bank.end()) {
                return;
            }
            active_pane = &*it;
            active_pane->cur.reset();
        }

        void switch_active() {
            if (active_pane == &pane_bank.back()) {
                auto it = std::find_if(
                    pane_bank.begin(), pane_bank.end(), [](Pane& p) { return p.is_leaf(); });
                switch_active(it->id);
            } else {
                auto it = std::find_if(pane_bank.begin(), pane_bank.end(), [this](Pane& p) {
                    return p.is_leaf() && p.id > active_pane->id;
                });

                switch_active(it->id);
            }
        }

        void close_active() {
            if (pane_bank.size() == 1) {
                return;
            }

            const int parent_id = active_pane->parent->id;
            const int current_pane_id = active_pane->id;

            if (get_pane(parent_id)->is_root()) {
                if (active_pane->is_leaf() && pane_bank.size() == 3) {
                    active_pane = get_pane(parent_id);
                    active_pane->kill_children();
                    pane_bank = {*get_pane(parent_id)};
                } else {
                    check_all(get_pane(parent_id), [this](Pane* p) {
                        if (p->is_leaf()) {
                            if (active_pane->parent->is_split_vertical()) {
                                p->dimensions.horizontal += std::min(
                                    active_pane->dimensions.horizontal, get_term_size().horizontal);
                            } else {
                                p->dimensions.vertical += std::min(
                                    active_pane->dimensions.vertical, get_term_size().vertical);
                            }
                        }
                    });

                    remove_from_bank(current_pane_id);
                    remove_from_bank(parent_id);
                    switch_active();
                }
            } else {
                if (active_pane->is_leaf()) {
                    active_pane = get_pane(parent_id);
                    remove_from_bank(active_pane->left->id);
                    remove_from_bank(active_pane->right->id);
                    active_pane->kill_children();
                } else {
                    check_all(get_pane(current_pane_id), [this](Pane* p) {
                        if (p->is_leaf()) {
                            if (active_pane->parent->is_split_vertical()) {
                                p->dimensions.horizontal += std::min(
                                    active_pane->dimensions.horizontal, get_term_size().horizontal);
                            } else {
                                p->dimensions.vertical += std::min(
                                    active_pane->dimensions.vertical, get_term_size().vertical);
                            }
                        }
                    });
                }
            }

            draw_all();
            active_pane->cur.move(active_pane->origin);
        }

        void draw_all() {
            clear_screen();

            std::for_each(pane_bank.begin(), pane_bank.end(), [](Pane p) {
                if (p.is_leaf()) {
                    p.draw();
                }
            });
        }

        void update() {
            active_pane->draw();
            active_pane->cur.reset();
        }
    };  // class PaneManager

}  // namespace rawterm
#endif  // RAWTERM_PANE_H
