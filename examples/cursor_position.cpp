#include <rawterm/core.h>
#include <rawterm/cursor.h>

#include <iostream>
#include <optional>

int main() {
    rawterm::enable_raw_mode();
    std::optional<rawterm::Pos> raw_loc = rawterm::Cursor::get_raw_location();
    std::cout << raw_loc.value_or(rawterm::Pos {0, 0}) << "\r\n";

    return 0;
}
