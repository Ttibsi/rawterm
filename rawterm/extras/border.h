#ifndef RAWTERM_BORDER_H
#define RAWTERM_BORDER_H

#include <algorithm>
#include <optional>
#include <span>
#include <string>
#include <vector>

#include <rawterm/color.h>
#include <rawterm/core.h>
#include <rawterm/cursor.h>
#include <rawterm/screen.h>

namespace rawterm {
    struct Border {
        Region size;
        std::size_t border_padding = 0;

        std::optional<char> border_char;
        std::optional<std::string> border_title;
        std::optional<Color> border_color;

        static constexpr std::string CORNER_TL = "\u250C";
        static constexpr std::string CORNER_TR = "\u2510";
        static constexpr std::string CORNER_BL = "\u2514";
        static constexpr std::string CORNER_BR = "\u2518";
        static constexpr std::string VERTICAL_BAR = "\u2502";
        static constexpr std::string HORIZONTAL_BAR = "\u2500";

        Border(Region size) : size(size), border_char(std::nullopt) {}
        Border(Region size, char border_char) : size(size), border_char(border_char) {}

        Border& set_padding(std::size_t padding) {
            border_padding = padding;
            return *this;
        }

        Border& set_title(const std::string& title) {
            border_title = title;
            return *this;
        }

        Border& set_color(const Color& color) {
            border_color = color;
            return *this;
        }

        [[nodiscard]] const std::string truncated_title() const {
            if (!(border_title.has_value())) {
                return "";
            }

            const std::size_t truncated_length = size.width() - 2;
            if (truncated_length < 4) {
                return "...";
            }

            if (truncated_length >= border_title.value().size()) {
                return border_title.value();
            }

            return border_title.value().substr(0, static_cast<std::size_t>(size.width() - 4)) +
                   "...";
        }

        [[nodiscard]] std::vector<std::string> render(std::span<std::string> text) const {
            auto trunc_title = truncated_title();
            std::vector<std::string> render = {""};
            std::size_t longest_txt =
                std::max_element(
                    text.begin(), text.end(),
                    [](const std::string& a, const std::string& b) { return a.size() < b.size(); })
                    ->size() +
                std::size_t(border_padding);

            if (longest_txt > size.width()) {
                longest_txt = size.width() - 2;
            }

            // Top line
            const std::size_t post_title_len =
                static_cast<std::size_t>(size.width() - border_padding) - trunc_title.size();
            if (border_char.has_value()) {
                render.at(0) = border_char.value() + trunc_title;
                render.at(0) += std::string(post_title_len, border_char.value());
            } else {
                render.at(0) = CORNER_TL + trunc_title;
                for (std::size_t i = 0; i < post_title_len; i++) {
                    render.at(0) += HORIZONTAL_BAR;
                }
                render.at(0) += CORNER_TR;
            }

            // Drawing text
            if (!text.empty()) {
                for (const auto& line : text) {
                    std::string rendered_line = "";
                    if (border_char.has_value()) {
                        rendered_line.push_back(border_char.value());
                    } else {
                        rendered_line += VERTICAL_BAR;
                    }

                    std::string drawable_text = line;
                    if (line.size() > static_cast<std::size_t>(longest_txt)) {
                        drawable_text = line.substr(0, static_cast<std::size_t>(longest_txt));
                    }
                    const std::size_t line_buffer =
                        size.width() - drawable_text.size() - border_padding - 2;
                    rendered_line += std::string(border_padding, ' ') + drawable_text +
                                     std::string(border_padding + line_buffer, ' ');

                    if (border_char.has_value()) {
                        rendered_line.push_back(border_char.value());
                    } else {
                        rendered_line += VERTICAL_BAR;
                    }

                    render.push_back(rendered_line);
                }
            }

            // Bottom line
            std::string btm = "";
            if (border_char.has_value()) {
                btm = std::string(render.at(0).size(), border_char.value());
            } else {
                btm = CORNER_BL;
                for (std::size_t i = 0; i <= size.width() - border_padding - 1; i++) {
                    btm += HORIZONTAL_BAR;
                }
                btm += CORNER_BR;
            }

            render.push_back(btm);

            return render;
        };

        void draw(Cursor& cur, std::span<std::string> text) const {
            // Disable if rawterm_debug
            if (detail::is_debug()) {
                return;
            }

            // Check if terminal size is reasonable and early return if not
            const Pos current_term_size = get_term_size();
            if (current_term_size.vertical < int32_t(size.height())) {
                return;
            } else if (current_term_size.horizontal < int32_t(size.width())) {
                return;
            }

            const std::vector<std::string> render_lines = render(text);
            for (int i = 0; i < static_cast<int>(render_lines.size()); i++) {
                cur.move({size.top_left.vertical + i, size.top_left.horizontal});

                if (border_color.has_value()) {
                    std::cout << "\x1b[" << border_color.value();
                }
                std::cout << render_lines.at(uint32_t(i)) << std::flush;
                if (border_color.has_value()) {
                    std::cout << Colors::reset;
                }
            }
        }
    };

}  // namespace rawterm

#endif  // RAWTERM_BORDER_H
