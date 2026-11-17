#include "core.h"

#include "include/ut.hpp"

boost::ut::suite<"Core"> core_suite = [] {
    using namespace boost::ut;

    // TODO: Handle multiple modifiers
    "getMod"_test = [] {
        should("Have no modifiers") = [] {
            rawterm::Key k = 'c';
            expect(k.getMod() == rawterm::Mod::None);
        };

        should("Have a modifier") = [] {
            auto k2 = rawterm::Key('c', rawterm::Mod::Arrow);
            expect(k2.getMod() == rawterm::Mod::Arrow);
        };
    };

    "isCharInput"_test = [] {
        should("Be a char") = [] {
            rawterm::Key k = 'c';
            expect(k.isCharInput() == true) << k.code << " " << rawterm::to_string(k.mod[0]);
        };

        should("Be a capital char") = [] {
            auto k = rawterm::Key('C', rawterm::Mod::Shift);
            expect(k.isCharInput() == true) << k.code << " " << rawterm::to_string(k.mod[0]);
        };

        should("Be an arrow") = [] {
            auto k = rawterm::Key('c', rawterm::Mod::Arrow);
            expect(k.isCharInput() == false);
        };

        should("Not be a char") = [] {
            auto k = rawterm::Key(' ', rawterm::Mod::Escape);
            expect(k.isCharInput() == false);
        };
    };

    "isSpace"_test = [] {
        should("Be a space") = [] {
            auto k = rawterm::Key(' ', rawterm::Mod::Space);
            expect(k.isSpace() == true);
        };

        should("Not be a space") = [] {
            auto k = rawterm::Key('4', rawterm::Mod::None);
            expect(k.isSpace() == false);
        };

        should("Code has multiple chars") = [] {
            auto k = rawterm::Key("£", rawterm::Mod::Shift, "");
            expect(k.isSpace() == false);
        };
    };
};
