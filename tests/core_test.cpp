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

    "key op=="_test = [] {
        should("Match identical unmodified keys") = [] {
            const rawterm::Key a('c');
            const rawterm::Key b('c');
            expect(a == b);
        };

        should("Treat None and an empty modifier list as equivalent") = [] {
            rawterm::Key a('c');
            rawterm::Key b('c');
            b.mod.clear();
            expect(a == b);
            expect(b == a);
        };

        should("Treat None and an empty modifier list as equivalent in either operand") = [] {
            rawterm::Key a('c');
            rawterm::Key b('c');
            a.mod.clear();
            expect(a == b);
            expect(b == a);
        };

        should("Treat None and empty modifiers as equivalent") = [] {
            rawterm::Key a('c');
            rawterm::Key b('c');
            b.mod.clear();
            expect(a == b);
            expect(b == a);
        };

        should("Match identical modified keys") = [] {
            const rawterm::Key a('C', rawterm::Mod::Shift);
            const rawterm::Key b('C', rawterm::Mod::Shift);
            expect(a == b);
        };

        should("Not match different codes") = [] {
            const rawterm::Key a('c');
            const rawterm::Key b('d');
            expect(!(a == b));
        };

        should("Not match different modifiers") = [] {
            const rawterm::Key a('c', rawterm::Mod::Shift);
            const rawterm::Key b('c', rawterm::Mod::Arrow);
            expect(!(a == b));
        };

        should("Ignore raw when code and modifiers match") = [] {
            const rawterm::Key a("x", rawterm::Mod::Shift, "raw-a");
            const rawterm::Key b("x", rawterm::Mod::Shift, "raw-b");
            expect(a == b);
        };

        should("Compare multi-character codes") = [] {
            const rawterm::Key a("£", rawterm::Mod::Shift, "");
            const rawterm::Key b("£", rawterm::Mod::Shift, "");
            const rawterm::Key c("€", rawterm::Mod::Shift, "");
            expect(a == b);
            expect(!(a == c));
        };

        should("Treat None as equivalent to empty modifiers in either operand") = [] {
            rawterm::Key none('c');
            rawterm::Key empty('c');
            empty.mod.clear();
            expect(none == empty);
            expect(empty == none);
        };

        should("Reject an empty modifier list on a modified key") = [] {
            rawterm::Key modified('c', rawterm::Mod::Shift);
            rawterm::Key empty('c');
            empty.mod.clear();
            expect(!(modified == empty));
            expect(!(empty == modified));
        };
    };
};
