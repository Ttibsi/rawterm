#include "screen.h"

#include "include/ut.hpp"

boost::ut::suite<"Screen"> screen_suite = [] {
    using namespace boost::ut;

    "partial_cmp"_test = [] {
        rawterm::Pos foo = {1, 1};
        rawterm::Pos bar = {1, 5};
        rawterm::Pos baz = {5, 1};
        rawterm::Pos quox = {5, 5};

        should("Same loc") = [&] { expect(foo.partial_cmp(foo) == true); };

        should("vertical match") = [&] { expect(foo.partial_cmp(bar) == true); };

        should("horizontal match") = [&] { expect(foo.partial_cmp(baz) == true); };

        should("no match") = [&] { expect(foo.partial_cmp(quox) == false); };
    };

    "to_str"_test = [] {
        rawterm::Pos foo = {1, 1};
        expect(foo.to_str() == "Pos(1, 1)");
    };

    "contains"_test = [] {
        auto r = rawterm::Region({1, 1}, {5, 5});

        expect(r.contains({2, 1}) == true);
        expect(r.contains({5, 6}) == false);
        expect(r.contains({6, 6}) == false);
    };

    "intersect"_test = [] {
        auto r = rawterm::Region({1, 1}, {5, 5});
        auto r2 = rawterm::Region({3, 3}, {8, 8});

        auto intersect = r.intersect(r2);
        expect(intersect.top_left.vertical = 3);
        expect(intersect.top_left.horizontal = 3);
        expect(intersect.bottom_right.vertical = 5);
        expect(intersect.bottom_right.horizontal = 5);
    };

    "height"_test = [] {
        auto r = rawterm::Region({1, 1}, {5, 5});
        expect(r.height() == 4);
    };

    "width"_test = [] {
        auto r = rawterm::Region({1, 1}, {5, 5});
        expect(r.width() == 4);
    };
};
