#include "color.h"

#include <string>

#include "include/ut.hpp"

boost::ut::suite<"Color"> color_suite = [] {
    using namespace boost::ut;

    "to_hex"_test = [] {
        rawterm::Color c;
        expect(c.to_hex() == "#000000");

        auto c2 = rawterm::Color("#123456");
        expect(c2.to_hex() == "#123456");

        auto c3 = rawterm::Color(18, 52, 86);
        expect(c3.to_hex() == "#123456");
    };

    "to_str"_test = [] {
        rawterm::Color c;
        expect(c.to_str() == "0;0;0m");

        auto c2 = rawterm::Color("#123456");
        expect(c2.to_str() == "18;52;86m");

        auto c3 = rawterm::Color(18, 52, 86);
        expect(c3.to_str() == "18;52;86m");
    };

    "set_foreground"_test = [] {
        auto c = rawterm::Color(0, 255, 0);
        std::string expected = "\x1B[38;2;0;255;0mHello\x1B[39m";
        expect(rawterm::set_foreground("Hello", c) == expected);
    };

    "set_background"_test = [] {
        auto c = rawterm::Color(0, 255, 0);
        std::string expected = "\x1B[48;2;0;255;0mHello\x1B[49m";
        expect(rawterm::set_background("Hello", c) == expected);
    };
};
