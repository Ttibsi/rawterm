#include "cursor.h"

#include "include/ut.hpp"

boost::ut::suite<"Cursor"> cursor_suite = [] {
    using namespace boost::ut;

    "Move"_test = [] {
        rawterm::Cursor cur;
        cur.move({3, 3});
        expect(cur.vertical == 3);
        expect(cur.horizontal == 3);
    };

    "move_up"_test = [] {
        rawterm::Cursor cur;
        cur.move({5, 5});

        should("move up by one pos") = [&] {
            cur.move_up();
            expect(cur.vertical == 4);
            expect(cur.horizontal == 5);
        };

        should("move up by multiple pos") = [&] {
            cur.move_up(2);
            expect(cur.vertical == 2);
            expect(cur.horizontal == 5);
        };
    };

    "move_down"_test = [] {
        rawterm::Cursor cur;
        cur.move({5, 5});

        should("move down by one pos") = [&] {
            cur.move_down();
            expect(cur.vertical == 6);
            expect(cur.horizontal == 5);
        };

        should("move down by multiple pos") = [&] {
            cur.move_down(2);
            expect(cur.vertical == 8);
            expect(cur.horizontal == 5);
        };
    };

    "move_right"_test = [] {
        rawterm::Cursor cur;
        cur.move({5, 5});

        should("move right by one pos") = [&] {
            cur.move_right();
            expect(cur.vertical == 5);
            expect(cur.horizontal == 6);
        };

        should("move right by multiple pos") = [&] {
            cur.move_right(2);
            expect(cur.vertical == 5);
            expect(cur.horizontal == 8);
        };
    };

    "move_left"_test = [] {
        rawterm::Cursor cur;
        cur.move({5, 5});

        should("move left by one pos") = [&] {
            cur.move_left();
            expect(cur.vertical == 5);
            expect(cur.horizontal == 4);
        };

        should("move left by multiple pos") = [&] {
            cur.move_left(2);
            expect(cur.vertical == 5);
            expect(cur.horizontal == 2);
        };
    };
};
