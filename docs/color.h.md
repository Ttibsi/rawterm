# color.h

Constructors:
* `Color();`
* `Color(int, int, int);`

Overloaded Operators:
* `operator<<`

* `[[nodiscard]] std::string set_foreground(const std::string&, const Color);`
* `[[nodiscard]] std::string set_background(const std::string&, const Color);`
* `[[nodiscard]] std::string reset_foreground();`
* `[[nodiscard]] std::string reset_background();`
* `void set_terminal_background(const Color&);`

Rawterm also provides preset default colors for the base16 color set, under the
`rawterm::Colors` namespace.

