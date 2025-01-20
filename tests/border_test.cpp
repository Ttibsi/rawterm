#include "extras/border.h"

#include "include/ut.hpp"

boost::ut::suite<"Border"> border_suite = [] {
    using namespace boost::ut;

    auto b = rawterm::Border(rawterm::Region({1, 1}, {5, 5}));
    auto b2 = rawterm::Border(rawterm::Region({1, 1}, {5, 5}), '#');
    auto b3 = rawterm::Border(rawterm::Region({1, 1}, {5, 5}));

    skip / "render without settings"_test = [&b, &b2] {};

    skip / "Set padding"_test = [&b] {};
    skip / "render with padding"_test = [&b, &b2] {};

    skip / "Set title"_test = [&b, &b2] {};
    skip / "render with padding and title"_test = [&b] {};
    skip / "render with title"_test = [&b2] {};

    skip / "Set color"_test = [&b, &b2, &b3] {};
    skip / "render with padding, title, color"_test = [&b] {};
    skip / "render with title, color"_test = [&b2] {};
    skip / "render with color"_test = [&b3] {};
};
