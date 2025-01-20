#include <rawterm/color.h>
#include <rawterm/core.h>
#include <rawterm/cursor.h>
#include <rawterm/extras/border.h>
#include <rawterm/screen.h>

int main() {
    rawterm::enable_raw_mode();
    rawterm::enter_alt_screen();
    rawterm::enable_signals();

    rawterm::Cursor cur;
    cur.reset();

    rawterm::Region region = rawterm::Region(rawterm::Pos(3, 3), rawterm::Pos(10, 10));

    auto border = rawterm::Border(region).set_padding(1).set_title("Hello world");
    border.draw(cur);

    rawterm::Region region2 = rawterm::Region(rawterm::Pos(3, 3), rawterm::Pos(10, 10));

    auto color = rawterm::Color(109, 192, 35);
    auto border2 = rawterm::Border(region2, '#').set_title("This is my title").set_color(color);
    border2.draw(cur);

    std::ignore = rawterm::wait_for_input();
    return 0;
}
