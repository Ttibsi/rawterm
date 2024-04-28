#ifndef RAWTERM_EXTRAS_PANE_H
#define RAWTERM_EXTRAS_PANE_H
 
#include <cassert>
#include <cstdlib>
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
        enum class Split { None, Down, Right };

        // template<typename T = std::vector<std::string> >
        struct Pane {
            std::optional<unsigned int> active_id;
            Pos origin;
            Pos dimensions;
            std::vector<std::string> content; // TODO: Template this?
            Cursor cur;
            std::optional<Color> background_color;
            Split split_direction = Split::None; 

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
                if (detail::is_debug()) { return; }
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
                if (detail::is_debug()) { return; }

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

            struct Closed_Pane {
                detail::Pane* pane;
                Pos old_dims;
            };

            Closed_Pane close_node(detail::Pane* n, bool left = false) {
                detail::Pane* parent = n->parent;
                if (parent == root.get()) {
                    root = std::move(left ? parent->right : parent->left);
                }

                if (left) {
                    parent->active_id = parent->right->active_id;
                    parent->content = parent->right->content;
                    parent->background_color = parent->right->background_color;
                } else {
                    parent->right = std::move(n->right); //TODO: fix

                    detail::Pane* l = parent->left.get();
                    while (!(l->active_id.has_value())) { l = l->left.get(); }

                    parent->active_id = l->active_id;
                    parent->content = l->content;
                    parent->background_color = l->background_color;
                }

                auto k = wait_for_input();

                parent->cur.move(parent->origin);
                parent->left = nullptr;
                parent->right = nullptr;

                return {parent, n->dimensions};
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
                    new_dims.horizontal + (odd_split ? 2 : 1)
                };

                // Needs to be before creating child leaves
                count++;
                active_pane->split_direction = detail::Split::Right;

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

                active_pane->left->parent = active_pane;
                active_pane->right->parent = active_pane;
                active_pane->active_id = {};

                active_pane->left->draw();
                active_pane->right->draw();
                active_pane = active_pane->right.get();

                return count;
            }

            // TODO: this copies instead of moves?
            const unsigned int  split_horizontal(std::vector<std::string> t) {
                Pos new_dims = {
                    active_pane->dimensions.vertical / 2,
                    active_pane->dimensions.horizontal 
                };
                bool odd_split = false;
                if (active_pane->dimensions.vertical % 2) { odd_split = true; }

                Pos split_start = {
                    new_dims.vertical + (odd_split ? 2 : 1),
                    active_pane->origin.horizontal,
                };

                // Needs to be before creating child leaves
                count++;
                active_pane->split_direction = detail::Split::Down;

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

                active_pane->left->parent = active_pane;
                active_pane->right->parent = active_pane;
                active_pane->active_id = {};

                active_pane->left->draw();
                active_pane->right->draw();
                active_pane = active_pane->right.get();

                return count;
            }

            const unsigned int split_vertical() { return split_vertical(std::vector<std::string>()); }
            const unsigned int split_horizontal() { return split_horizontal(std::vector<std::string>()); }

            const unsigned int close_active() {
                if (active_pane == root.get()) { return 0; }

                detail::Pane* p = active_pane;
                bool left = (p->parent->left.get() == p);

                if (p->parent == root.get()) {
                    if (left) {
                        if (p->parent->right != nullptr) { root = std::move(p->parent->right); }
                    } else {
                        if (p->parent->left != nullptr) { root = std::move(p->parent->left); }
                    }
                }

                Pos closing_dims = p->dimensions;
                detail::Split direction = p->split_direction;

               if (left) {
                    detail::Pane* n = p->right.get();
                    while (!(n->active_id.has_value())) { n = n->right.get(); }
                    n->dimensions += (direction == detail::Split::Down ? closing_dims.vertical : closing_dims.horizontal);
                } else {
                    detail::Pane* n = p->left.get();
                    while (!(n->active_id.has_value())) { n = n->left.get(); }
                    n->dimensions += (direction == detail::Split::Down ? closing_dims.vertical : closing_dims.horizontal);
                }

                assert(root->dimensions == get_term_size());
                active_pane = p;
                draw_all();

                count--;
                return count;


                // Closed_Pane ret = {};
                //
                // // Close the correct node
                // if (p->parent->left.get() == p) {
                //     ret = close_node(p, true);
                // } else {
                //    while (p->left != nullptr) {
                //         p = p->left.get();
                //     } 
                //     ret = close_node(p, false);
                // }
                //
                //
                // p = ret.pane;
                // int old_vertical = ret.old_dims.vertical;
                // int old_horizontal = ret.old_dims.horizontal;
                //
                // // TODO: What if it's the size of the terminal (the old root)
                // if (p->split_direction == detail::Split::Down) {
                //     recursive_search(p, [old_vertical](detail::Pane* n){
                //         if (n->dimensions.vertical + old_vertical > get_term_size().vertical) {
                //             n->origin.vertical -= (old_vertical / 2);
                //         } else {
                //             n->dimensions.vertical += (old_vertical / 2);
                //         }
                //
                //         return false;
                //     });
                // } else if (p->split_direction == detail::Split::Right) {
                //     recursive_search(p, [old_horizontal](detail::Pane* n){
                //         if (n->dimensions.horizontal + old_horizontal > get_term_size().horizontal) {
                //             n->origin.horizontal -= (old_horizontal / 2);
                //         } else {
                //             n->dimensions.horizontal += (old_horizontal / 2);
                //         }
                //
                //         return false;
                //     });
                // }
                // 
                // active_pane = p;
                // draw_all();
                //
                // count--;
                // return count;
            }

            void switch_active(unsigned int pane_idx) {
                recursive_search(root.get(), [pane_idx](detail::Pane* n){
                    return n->active_id == pane_idx;
                });
                
                active_pane->draw();
            }

            void switch_active() {
                if (active_pane->active_id.value() == count) {
                    switch_active(active_pane->active_id.value() - 1);
                } else {
                    switch_active(active_pane->active_id.value() + 1);
                }
                active_pane->cur.reset();
            }

            void draw_all() {
                detail::Pane* p = active_pane;
                clear_screen();
                p->cur.move({1,1});

                recursive_search(root.get(), [](detail::Pane* n){
                    n->draw();
                    return true;
                });

                active_pane = p;
            }
    };
}

#endif //RAWTERM_EXTRAS_PANE_H
