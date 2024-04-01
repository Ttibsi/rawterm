#include "extras.h"

#include <rawterm/core.h>
#include <rawterm/cursor.h>
#include <rawterm/text.h>

namespace rawterm {
    const std::string set_header(const std::string& title) {
        rawterm::Pos term_size = rawterm::get_term_size();
        int half_screen_len = (term_size.horizontal / 2) - (title.size() / 2);

        auto c = rawterm::Cursor();
        c.reset();

        std::string ret = rawterm::inverse(std::string(half_screen_len, ' '));
        ret += rawterm::inverse(title);
        ret += rawterm::inverse(std::string(half_screen_len, ' '));

        return ret;
    }
}
