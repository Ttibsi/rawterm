#include "extras/menu.h"

#include <string>
#include <vector>

#include "include/ut.hpp"

boost::ut::suite<"Menu"> menu_suite = [] {
    using namespace boost::ut;

    auto menu_region = rawterm::Region({3, 3}, {8, 8});
    std::vector<std::string> options = {"Hello world", "Foo", "Bar", "This is some text"};
    auto menu = rawterm::VMenu(menu_region, options);

    "next"_test = [&menu] {
        expect(menu.active_opt == 0);
        expect(menu.get_current_opt() == "Hello world");

        menu.next();
        menu.next();

        expect(menu.active_opt == 2);
        expect(menu.get_current_opt() == "Bar");
    };

    "prev"_test = [&menu] {
        expect(menu.active_opt == 2);
        expect(menu.get_current_opt() == "Bar");

        menu.prev();
        menu.prev();

        expect(menu.active_opt == 0);
        expect(menu.get_current_opt() == "Hello world");
    };

    "get_current_opt"_test = [&menu] {
        expect(menu.get_current_opt() == "Hello world");

        menu.next();

        expect(menu.get_current_opt() == "Foo");
    };

    "add_opt"_test = [&menu] {
        for (int i = 0; i < 10; i++) {
            menu.next();
        }
        expect(menu.active_opt = 3);

        std::string dean_winchester = "Saving People, Hunting Things, The family business";
        menu.add_opt(dean_winchester);

        expect(menu.get_current_opt() == "This is some text");

        menu.next();
        expect(menu.active_opt = 4);
        expect(menu.get_current_opt() == dean_winchester);
    };

    "vertical render"_test = [&menu, &options] {
        std::string render = menu.render();
        const int nl_count = std::count(render.begin(), render.end(), '\n');
        expect(nl_count == options.size() + 1);
    };
};
