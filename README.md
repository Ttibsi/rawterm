# rawterm
A c++20 library for working with raw mode in a terminal. See the
`examples/` directory for examples on how to use this library, and each
header file in `rawterm/` has a relevant markdown file in `docs/` when
relevant

See `examples/hello_world.cpp` for a simple place to start.

### How to use
As of `v4.0.0`, rawterm can be imported with cmake fetchcontent. Add the
following passage to your CMakeLists.txt file then link to the `rawterm`
library using `target_link_libraries`

```cmake
include(FetchContent)
fetchcontent_declare(
  rawterm
  GIT_REPOSITORY https://github.com/ttibsi/rawterm
  GIT_TAG        25989a41742bd4105a796c9607bb88260cd682fb
)
fetchcontent_makeavailable(rawterm)
```

Alternatively, if you use `zig` as your build system, you can run

```console
zig fetch --save=rawterm git+https://github.com/ttibsi/rawterm
```

This has only been tested with zig 0.13.0

### New in version: 4.0.7
* Added unit testing in `tests/` directory
* Added zig build option
* `extras/menu.h` - New widget added for adding a vertical scrolling menu
* `extras/border.h` - New widget added for drawing borders around a given
vector of strings
* `screen.h` - Added hashing for `Pos` object to use as a key in
`std::unordered_map` and other hashed objects
* `text.h` - Add `raw_str` function to strip a string of any added ansi escape
codes
* Added new examples:
    * `examples/vertical_menu.cpp` for trialing menu.h
    * `examples/game.cpp` porting a simple "collect the coin" game to Rawterm
    * `examples/borders.cpp` for a simple display of how to use border.h

### Projects Using Rawterm
* [iris](https://github.com/ttibsi/iris) - Modal terminal text editor
* [re-view](https://github.com/ttibsi/re-view) - Regex viewer (uses the old header-only library)
* [gol-tui](https://github.com/ttibsi/gol-tui) - Conway's Game of Life (uses zig build system)

Feel free to add any projects written using rawterm to this list via PR!
