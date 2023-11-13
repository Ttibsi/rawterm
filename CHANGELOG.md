### v2.2.4
* Fixed cursor changes not flushing the escape code
* Added `rawterm::to_string` to get string values of the `rawterm::Mod` enum

### v2.2.3
* Updated `offset_cursor` to used signed ints instead of a `rawterm::Pos`

### v2.2.2
* Fixed incorrect naming of `Pos` members

### v2.2.1
* Inlined some functions to prevent "duplicate symbol" linker issue

### v2.2.0
* `key.mod` is now a `std::deque` instead of an std::vector
* Added `rawterm::Mod::None` as a default value
* Added `getMod()` to read modifiers sequentially
* Fixed error with `alt_l + capital letter` as it was using multiple modifiers
    * `Alt_L` and `Shift`

### v2.1.0
* Added `isCharInput()` function
* ^m now returns `Enter` modifier value instead of `Control`
* `Pos` struct members renamed to be clearer

### v2.0.0
* Added descriptive comments to each function
* Added `Pos` structure, now returned by `get_term_size` and accepted by
`move_cursor` (eczbek - #10)
* Made `asciiLetters` into an unordered set for better accessing (eczbek - #10)
* Replaced if-else chains with switches (eczbek - #10)
* Wrapped everything in `rawterm` namespace (eczbek - #10)

### v1.7.1
* Remove unneccesary copying (@Eczbek - #9)

### v1.7.0
* Fixed occasional segfault with reading `esc` char

### v1.6.0
* Added the backspace key detection

### v1.5.0
*  `get_term_size()` now returns a pair of std::size_t instead of ints
* `Mod` is now an `enum class` instead of an `enum`
* Added cursor appearance functions

### v1.4.0
* save and load cursor position (SCO sequences)

### v1.3.1
* Exported formatting functions ("header" section of header)

### v1.3.0
* Added text formatting functions

### v1.2.1
* Fix error with move_cursor() not actually updating

### v1.2.0
* Added  `move_cursor()`
* Added  `clear_screen()`

### v1.1.0
* Added `get_term_size()`
