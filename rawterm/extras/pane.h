#ifndef RAWTERM_EXTRAS_PANE_H
#define RAWTERM_EXTRAS_PANE_H

#include <functional>
#include <iostream>
#include <memory>
#include <optional>
#include <vector>

#include <rawterm/color.h>
#include <rawterm/core.h>
#include <rawterm/cursor.h>

namespace rawterm {
    namespace detail {
        // template<typename T = std::vector<std::string> >
        struct Pane {
            std::optional<unsigned int> active_id;
            const Pos origin;
            const Pos dimensions;
            std::vector<std::string> content; // TODO: Template this?
            Cursor cur;
            std::optional<Color> background_color;

            Pane* parent = nullptr;
            std::unique_ptr<Pane> left = nullptr;
            std::unique_ptr<Pane> right = nullptr;

            Pane(unsigned int id): Pane(std::vector<std::string>(), id) {}

            Pane(std::vector<std::string> t, unsigned int id): 
                active_id(id), origin(Pos{1,1}), dimensions(get_term_size()), content(t), cur(Cursor(origin)) {}

            // For splitting
            Pane(unsigned int id, Pos new_origin, Pos dims, std::vector<std::string> new_content, Cursor cur, std::optional<Color> col):
                active_id(id), origin(new_origin), dimensions(dims), content(new_content), cur(cur), background_color(col) {}

            void clear() {
                Pos end = origin + dimensions;

                for (int i = origin.vertical; i < end.vertical; i++) {
                    for (int j = origin.horizontal; j < end.horizontal; j++) {
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
                cur.move(origin);

                // TODO: Modify this for termplate type
                for (int idx = 0; idx < content.size(); idx++) {
                    if (idx >= dimensions.vertical) { break; }

                    std::string out = content[idx];
                    if (out.size() > dimensions.horizontal) {
                        out = out.substr(0, dimensions.horizontal);
                    }
                    
                    if (background_color.has_value()) {
                        std::cout << "\x1B[48;2;" << background_color.value();
                    }
                    std::cout << out;
                    cur.move({origin.vertical + idx + 1, origin.horizontal});
                }

                cur.move(origin);
            }
        };
    }  // namespace detail

    class PaneManager {
        private:
            detail::Pane* active_pane;
            std::unique_ptr<detail::Pane> root;
            unsigned int count;

            void recursive_search(detail::Pane* node, std::function<bool(detail::Pane*)> f) {
                if (node->left != nullptr) {
                    // we aren't at a leaf node
                    recursive_search(node->left.get(), f);
                    recursive_search(node->right.get(), f);
                    return;
                }

                if (f(node)) {
                    active_pane = node;
                }
            }

        public:
            PaneManager() : root(std::make_unique<detail::Pane>(1)), count(1) {
                active_pane = root.get();
            }

            [[nodiscard]] const unsigned int get_count() { return count; }

            void set_content(std::vector<std::string> t) {
                active_pane->content = t;
                active_pane->draw();
            }

            void set_pane_background(const Color& col) {
                active_pane->background_color = col;
                active_pane->draw();
            }

            const unsigned int split_vertical(std::vector<std::string> t) {
                Pos new_dims = {
                    active_pane->dimensions.vertical,
                    active_pane->dimensions.horizontal / 2
                };
                bool odd_split = false;
                if (active_pane->dimensions.horizontal % 2) { odd_split = true; }

                Pos split_start = {
                    active_pane->origin.vertical,
                    new_dims.horizontal + 1
                };
    
                count++;

                active_pane->left = std::make_unique<detail::Pane>(
                    active_pane->active_id.value(),
                    active_pane->origin, 
                    odd_split ? Pos{new_dims.vertical, new_dims.horizontal + 1} : new_dims,
                    active_pane->content,
                    active_pane->cur,
                    active_pane->background_color
                );

                active_pane->right = std::make_unique<detail::Pane>(
                    count,
                    split_start,
                    new_dims,
                    t,
                    Cursor(split_start),
                    active_pane->background_color
                );

                active_pane->active_id = {};
                active_pane->left->draw();
                active_pane->right->draw();
                active_pane = active_pane->right.get();

                return count;
            }

            const unsigned int  split_horizontal(std::vector<std::string> t) {
                Pos new_dims = {
                    active_pane->dimensions.vertical / 2,
                    active_pane->dimensions.horizontal 
                };
                bool odd_split = false;
                if (active_pane->dimensions.vertical % 2) { odd_split = true; }

                Pos split_start = {
                    new_dims.vertical + 1,
                    active_pane->origin.horizontal,
                };

                count++;

                active_pane->left = std::make_unique<detail::Pane>(
                    active_pane->active_id.value(),
                    active_pane->origin, 
                    odd_split ? Pos{new_dims.vertical + 1, new_dims.horizontal} : new_dims,
                    active_pane->content,
                    active_pane->cur,
                    active_pane->background_color
                );

                active_pane->right = std::make_unique<detail::Pane>(
                    count,
                    split_start,
                    new_dims,
                    t,
                    Cursor(split_start),
                    active_pane->background_color
                );

                active_pane->active_id = {};
                active_pane->left->draw();
                active_pane->right->draw();
                active_pane = active_pane->right.get();

                count++;
                return count;
            }

            const unsigned int split_vertical() { return split_vertical(std::vector<std::string>()); }
            const unsigned int split_horizontal() { return split_horizontal(std::vector<std::string>()); }

            const unsigned int close_active() {
                bool left_node = false;
                if (active_pane->parent->left.get() == active_pane) { left_node = true; }

                if (left_node) {
                    active_pane->parent->active_id = active_pane->parent->right->active_id;
                    active_pane->parent->content = active_pane->parent->right->content;
                    active_pane->parent->background_color = active_pane->parent->right->background_color;
                } else {
                    active_pane->parent->active_id = active_pane->parent->left->active_id;
                    active_pane->parent->content = active_pane->parent->left->content;
                    active_pane->parent->background_color = active_pane->parent->left->background_color;
                }

                active_pane = active_pane->parent;
                active_pane->draw();

                count--;
                return count;
            }

            void switch_active(unsigned int pane_idx) {
                recursive_search(root.get(), [pane_idx](detail::Pane* n){
                    return n->active_id == pane_idx;
                });
            }

            void switch_active() {
                switch_active(active_pane->active_id.value() + 1);
            }

            void draw_all() {
                detail::Pane* p = active_pane;

                recursive_search(root.get(), [](detail::Pane* n){
                    n->draw();
                    return true;
                });

                active_pane = p;
            }
    };
}

#endif //RAWTERM_EXTRAS_PANE_H
