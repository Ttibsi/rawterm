#include "extras/border.h"

#include <string>
#include <vector>

#include "include/ut.hpp"

boost::ut::suite<"Border"> border_suite = [] {
    using namespace boost::ut;

    auto b = rawterm::Border(rawterm::Region({1, 1}, {10, 10}));
    auto b2 = rawterm::Border(rawterm::Region({1, 1}, {10, 10}), '#');
    auto b3 = rawterm::Border(rawterm::Region({1, 1}, {10, 10}));
    std::vector<std::string> text = {
        "Lorem ipsum dolor sit amet", "consectetur adipiscing elit.", "Morbi ipsum ex semper",
        "placerat quam finibus sollicitudin. Nullam condimentum tellus ante"};

    "render without settings"_test = [&b, &text] {
        std::vector<std::string> expected = {"┌─────────┐", "│Lorem i│", "│consect│",
                                             "│Morbi i│",   "│placera│", "└─────────┘"};

        auto rendered = b.render(&text);
        expect(rendered == expected);
        // either end is a box drawing char, which is 3 bytes
        expect(rendered.at(1).size() == 13);
    };

    "Set padding"_test = [&b] {
        expect(b.border_padding == 0);
        b.set_padding(1);
        expect(b.border_padding == 1);
    };

    "render with padding"_test = [&b, &text] {
        std::vector<std::string> expected = {"┌────────┐", "│ Lorem i│", "│ consect│",
                                             "│ Morbi i│", "│ placera│", "└────────┘"};

        auto rendered = b.render(&text);
        expect(rendered == expected);

        // either end is a box drawing char, which is 3 bytes
        expect(rendered.at(1).size() == 14);
    };

    "Set title"_test = [&b, &b2] {
        b.set_title("Super long title please");
        expect(b.border_title == "Super long title please");

        b2.set_title("Test");
        expect(b2.border_title == "Test");
    };

    "truncated title"_test = [&b, &b2] {
        expect(b.truncated_title() == "Super...");
        expect(b2.truncated_title() == "Test");
    };

    "render with padding and title"_test = [&b, &text] {
        auto rendered = b.render(&text);
        std::string expected = "┌Super...┐";

        expect(rendered.at(0) == expected);
    };

    "render with title"_test = [&b2, &text] {
        auto rendered = b2.render(&text);
        std::string expected = "#Test#####";

        expect(rendered.at(0) == expected);
    };

    "Set color"_test = [&b, &b2, &b3] {
        b.set_color(rawterm::Color("#181818"));
        b2.set_color(rawterm::Color(255, 255, 255));
        b3.set_color(rawterm::Color("#FF0000"));

        expect(b.border_color.value() == rawterm::Color("#181818"));
        expect(b2.border_color.value() == rawterm::Color(255, 255, 255));
        expect(b3.border_color.value() == rawterm::Color("#FF0000"));
    };

    "render with padding, title, color"_test = [&b, &text] {
        auto rendered = b.render(&text);
        std::string expected = "┌Super...┐";

        expect(rendered.at(0) == expected) << rendered.at(0);
    };

    // "render with title, color"_test = [&b2, &text] {
    //     auto rendered = b2.render(&text);
    //     std::string expected = "";
    //
    //     expect(rendered.at(0) == expected);
    // };
    //
    // "render with color"_test = [&b3, &text] {
    //     auto rendered = b3.render(&text);
    //     std::string expected = "";
    //
    //     expect(rendered.at(0) == expected);
    // };
};
