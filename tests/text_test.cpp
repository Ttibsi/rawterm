#include "text.h"

#include <string>

#include "include/ut.hpp"

boost::ut::suite<"Text"> text_suite = [] {
    using namespace boost::ut;

    std::string s = "\x1b[32mHello\x1b[0m";

    "raw_at"_test = [&] {
        expect(rawterm::raw_at(s, 0) == 'H');
        expect(rawterm::raw_at(s, 1) == 'e');
        expect(rawterm::raw_at(s, 2) == 'l');
        expect(rawterm::raw_at(s, 3) == 'l');
        expect(rawterm::raw_at(s, 4) == 'o');

        expect(throws<std::out_of_range>([s] { std::ignore = rawterm::raw_at(s, 5); }));
    };

    // TODO: Do we want to test for multiple ansi escapes
    // TODO: Test buried ansi in the middle
    "raw_str"_test = [&] { expect(rawterm::raw_str(s) == "Hello"); };

    "raw_size"_test = [&] { expect(rawterm::raw_size(s) == 5); };
};
