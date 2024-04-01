# rawterm
A c++20 library for working with raw mode in a terminal. See the
`examples/` directory for examples on how to use this library, and the 
`rawterm/` library files for docs on how to use each function.

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

### Be Aware Of:
* `^m` and `enter` are the same key - they both pass `\x0d` code to the terminal
* `^i` and `tab` are the same key - they both pass `\x09` code to the terminal
* left alt and right alt are different keys - only left-alt has been handled
* `CMD` on macos doesn't return a unique ascii code, and is treated like the
base character being pressed

### Projects Using Rawterm
