#ifndef RAWTERM_TEXT_H
#define RAWTERM_TEXT_H

#include <string>

namespace rawterm {
    [[nodiscard]] std::string bold(const std::string &s);
    [[nodiscard]] std::wstring bold(const std::wstring &s);

    [[nodiscard]] std::string italic(const std::string &s);
    [[nodiscard]] std::wstring italic(const std::wstring &s);

    [[nodiscard]] std::string underline(const std::string &s);
    [[nodiscard]] std::wstring underline(const std::wstring &s);

    [[nodiscard]] std::string blink(const std::string &s);
    [[nodiscard]] std::wstring blink(const std::wstring &s);

    [[nodiscard]] std::string inverse(const std::string &s);
    [[nodiscard]] std::wstring inverse(const std::wstring &s);

    [[nodiscard]] std::string hidden(const std::string &s);
    [[nodiscard]] std::wstring hidden(const std::wstring &s);

    [[nodiscard]] std::string strikethrough(const std::string &s);
    [[nodiscard]] std::wstring strikethrough(const std::wstring &s);

    [[nodiscard]] const char raw_at(const std::string&, std::size_t);
    [[nodiscard]] const int raw_size(const std::string&);
}

#endif //RAWTERM_TEXT_H
