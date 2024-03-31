#ifndef RAWTERM_TEXT_H
#define RAWTERM_TEXT_H

#include <string>

namespace rawterm {
    std::string bold(const std::string &s);
    std::wstring bold(const std::wstring &s);

    std::string italic(const std::string &s);
    std::wstring italic(const std::wstring &s);

    std::string underline(const std::string &s);
    std::wstring underline(const std::wstring &s);

    std::string blink(const std::string &s);
    std::wstring blink(const std::wstring &s);

    std::string inverse(const std::string &s);
    std::wstring inverse(const std::wstring &s);

    std::string hidden(const std::string &s);
    std::wstring hidden(const std::wstring &s);

    std::string strikethrough(const std::string &s);
    std::wstring strikethrough(const std::wstring &s);
}

#endif //RAWTERM_TEXT_H
