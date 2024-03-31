#include "text.h"

namespace rawterm {
    std::string bold(const std::string &s) { return "\x1B[1m" + s + "\x1B[22m"; }
    std::wstring bold(const std::wstring &s) { return L"\x1B[1m" + s + L"\x1B[22m"; }

    std::string italic(const std::string &s) { return "\x1B[3m" + s + "\x1B[23m"; }
    std::wstring italic(const std::wstring &s) { return L"\x1B[3m" + s + L"\x1B[23m"; }

    std::string underline(const std::string &s) { return "\x1B[4m" + s + "\x1B[24m"; }
    std::wstring underline(const std::wstring &s) { return L"\x1B[4m" + s + L"\x1B[24m"; }

    std::string blink(const std::string &s) { return "\x1B[5m" + s + "\x1B[25m"; }
    std::wstring blink(const std::wstring &s) { return L"\x1B[5m" + s + L"\x1B[25m"; }

    std::string inverse(const std::string &s) { return "\x1B[7m" + s + "\x1B[27m"; }
    std::wstring inverse(const std::wstring &s) { return L"\x1B[7m" + s + L"\x1B[27m"; }

    std::string hidden(const std::string &s) { return "\x1B[8m" + s + "\x1B[28m"; }
    std::wstring hidden(const std::wstring &s) { return L"\x1B[8m" + s + L"\x1B[28m"; }

    std::string strikethrough(const std::string &s) { return "\x1B[9m" + s + "\x1B[29m"; }
    std::wstring strikethrough(const std::wstring &s) { return L"\x1B[9m" + s + L"\x1B[29m"; }
}
