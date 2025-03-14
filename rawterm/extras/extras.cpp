#include "extras.h"

#include <rawterm/core.h>
#include <rawterm/cursor.h>
#include <rawterm/text.h>

namespace rawterm {
    const std::string set_header(const std::string& title) {
        rawterm::Pos term_size = rawterm::get_term_size();
        std::size_t half_screen_len =
            static_cast<unsigned int>(term_size.horizontal / 2) - (title.size() / 2);
        bool odd = term_size.horizontal % 2;

        auto c = rawterm::Cursor();
        c.reset();

        std::string ret = rawterm::inverse(std::string(half_screen_len, ' '));
        ret += rawterm::inverse(title);
        if (odd) {
            ret += rawterm::inverse(" ");
        }
        ret += rawterm::inverse(std::string(half_screen_len, ' '));

        return ret;
    }
}  // namespace rawterm
