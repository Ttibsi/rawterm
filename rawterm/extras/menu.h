#ifndef RAWTERM_MENUS_H
#define RAWTERM_MENUS_H

#include <algorithm>
#include <string>
#include <string_view>
#include <vector>

#include "screen.h"
#include "text.h"

namespace rawterm {
    class Menu {
       protected:
        std::vector<std::string> opts = {};

       public:
        int active_opt = 0;
        Region dims;

        Menu(Region r) : dims(r) {};
        Menu(Region r, std::vector<std::string> passed_opts) : opts(passed_opts), dims(r) {}

        void next() {
            if (active_opt < opts.size() - 1) {
                active_opt++;
            }
        }

        void prev() {
            if (active_opt > 0) {
                active_opt--;
            }
        }

        std::string_view get_current_opt() const { return opts.at(active_opt); }

        // NOTE: Should this be protected?
        virtual std::string render() const = 0;
    };

    class VMenu final : public Menu {
       public:
        bool border = false;

        VMenu(Region r) : Menu(r) {}
        VMenu(Region r, std::vector<std::string> passed_opts) : Menu(r, passed_opts) {}

        std::string render() const override {
            std::string ret = "";
            const int longest_txt = std::max_element(
                                        opts.begin(), opts.end(),
                                        [](const std::string& a, const std::string& b) {
                                            return a.length() < b.length();
                                        })
                                        ->size();

            int view_size = std::min(
                static_cast<int>(opts.size()),
                (dims.bottom_right.vertical - dims.top_left.vertical));

            if (border) {
                ret += "\u250c";
                for (int i = 0; i < longest_txt; i++) {
                    ret += "\u2500";
                }
                ret += "\u2510\r\n";
            }

            for (int i = 0; i < view_size; i++) {
                // TODO: truncate to region
                if (i == active_opt) {
                    ret += " " + inverse(opts.at(i)) + "\r\n";
                } else {
                    ret += " " + opts.at(i) + "\r\n";
                }
            }

            if (border) {
                ret += "\u2514";
                for (int i = 0; i < longest_txt; i++) {
                    ret += "\u2500";
                }
                ret += "\u2518\r\n";
            }

            return ret;
        };
    };

    // class HMenu final: public Menu {
    //     public:
    //         HMenu(Region r): Menu(r) {}
    //         HMenu(Region r, std::vector<std::string> passed_opts): Menu(r, passed_opts) {}

    //         std::string render() const override {};
    // };

}  // namespace rawterm

#endif  // RAWTERM_MENUS_H
