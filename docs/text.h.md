# text.h
Functions for manipulating the appearance of text in the terminal, with 
overloads for both standard and wide strings.


* `[[nodiscard]] std::string bold(const std::string &s);`
* `[[nodiscard]] std::wstring bold(const std::wstring &s);`
* `[[nodiscard]] std::string italic(const std::string &s);`
* `[[nodiscard]] std::wstring italic(const std::wstring &s);`
* `[[nodiscard]] std::string underline(const std::string &s);`
* `[[nodiscard]] std::wstring underline(const std::wstring &s);`

* `[[nodiscard]] std::string blink(const std::string &s);`
* `[[nodiscard]] std::wstring blink(const std::wstring &s);`
Make the given text flash at a standard rate

* `[[nodiscard]] std::string inverse(const std::string &s);`
* `[[nodiscard]] std::wstring inverse(const std::wstring &s);`
Make the given text switch foreground and background colours

* `[[nodiscard]] std::string hidden(const std::string &s);`
* `[[nodiscard]] std::wstring hidden(const std::wstring &s);`
Make the given text invisible

* `[[nodiscard]] std::string strikethrough(const std::string &s);`
* `[[nodiscard]] std::wstring strikethrough(const std::wstring &s);`

* `[[nodiscard]] const char raw_at(const std::string&, const int);`
Get the position of a character in a string, disregarding ansi escape codes

* `[[nodiscard]] const int raw_size(const std::string&);`
Get the size of a string, disregarding ansi escape codes
