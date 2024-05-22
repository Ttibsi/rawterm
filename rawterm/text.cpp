#include <regex>

#include "text.h"

namespace rawterm {
    [[nodiscard]] std::string bold(const std::string &s) { return "\x1B[1m" + s + "\x1B[22m"; }
    [[nodiscard]] std::wstring bold(const std::wstring &s) { return L"\x1B[1m" + s + L"\x1B[22m"; }

    [[nodiscard]] std::string italic(const std::string &s) { return "\x1B[3m" + s + "\x1B[23m"; }
    [[nodiscard]] std::wstring italic(const std::wstring &s) { return L"\x1B[3m" + s + L"\x1B[23m"; }

    [[nodiscard]] std::string underline(const std::string &s) { return "\x1B[4m" + s + "\x1B[24m"; }
    [[nodiscard]] std::wstring underline(const std::wstring &s) { return L"\x1B[4m" + s + L"\x1B[24m"; }

    [[nodiscard]] std::string blink(const std::string &s) { return "\x1B[5m" + s + "\x1B[25m"; }
    [[nodiscard]] std::wstring blink(const std::wstring &s) { return L"\x1B[5m" + s + L"\x1B[25m"; }

    [[nodiscard]] std::string inverse(const std::string &s) { return "\x1B[7m" + s + "\x1B[27m"; }
    [[nodiscard]] std::wstring inverse(const std::wstring &s) { return L"\x1B[7m" + s + L"\x1B[27m"; }

    [[nodiscard]] std::string hidden(const std::string &s) { return "\x1B[8m" + s + "\x1B[28m"; }
    [[nodiscard]] std::wstring hidden(const std::wstring &s) { return L"\x1B[8m" + s + L"\x1B[28m"; }

    [[nodiscard]] std::string strikethrough(const std::string &s) { return "\x1B[9m" + s + "\x1B[29m"; }
    [[nodiscard]] std::wstring strikethrough(const std::wstring &s) { return L"\x1B[9m" + s + L"\x1B[29m"; }

    [[nodiscard]] const char raw_at(const std::string& str, const int index) {
        std::regex ansi_escape_code("\x1b\\[[0-9;]*[A-Za-z]");
        std::smatch match;
        int visible_index = 0;
        int pos = 0;

        while (pos < str.length()) {
            // Check if there's an ANSI escape code at the current position
            if (std::regex_search(str.begin() + pos, str.end(), match, ansi_escape_code) && match.position() == 0) {
                // Move position past the ANSI escape code
                pos += match.length();
            } else {
                // If the current visible index matches the target index, return the character
                if (visible_index == index) {
                    return str[pos];
                }
                // Move to the next character
                ++visible_index;
                ++pos;
            }
        }

        throw std::out_of_range("Index out of range");
    }

    [[nodiscard]] const int raw_size(const std::string& str) {
        std::regex ansi_escape_code("\x1b\\[[0-9;]*[A-Za-z]");
        std::string cleaned_str = std::regex_replace(str, ansi_escape_code, "");
        return cleaned_str.length();
    }
}
