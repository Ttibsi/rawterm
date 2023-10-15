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
