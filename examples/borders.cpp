#include <rawterm/color.h>
#include <rawterm/core.h>
#include <rawterm/cursor.h>
#include <rawterm/extras/border.h>
#include <rawterm/screen.h>

#include <string>
#include <vector>

int main() {
    rawterm::enable_raw_mode();
    rawterm::enter_alt_screen();
    auto signals_thread = rawterm::enable_signals();

    rawterm::Cursor::cursor_hide();
    rawterm::Cursor cur;
    cur.reset();

    std::vector<std::string> text = {"Hello world", "foo", "bar", "some text again and again"};
    rawterm::Region region = rawterm::Region(rawterm::Pos(3, 3), rawterm::Pos(10, 25));
    auto border = rawterm::Border(region).set_padding(1).set_title("Hello world");
    border.draw(cur, &text);

    rawterm::Region region2 = rawterm::Region(rawterm::Pos(13, 3), rawterm::Pos(20, 40));

    auto color = rawterm::Color(109, 192, 35);
    auto border2 = rawterm::Border(region2, '#').set_title("This is my title").set_color(color);
    border2.draw(cur, &text);

    std::ignore = rawterm::wait_for_input();
    rawterm::Cursor::cursor_show();
    signals_thread.join();
    return 0;
}
