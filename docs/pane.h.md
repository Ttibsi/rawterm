# extras/pane.h

A tmux-style pane management system useful for organising splits and
sub-windows in a terminal. This uses a Binary Space Partitioning tree
to manage the splits, with only leaf nodes being drawn to the screen.

Constructors:
* `PaneManager(const Pos&);`
    - This constructor takes the dimensions of the original terminal window

Public methods:
* `void print_all()`
    - Used for debugging, can print out information on every pane

* `[[nodiscard]] unsigned int count()`
    - Get the count of panes

* `[[nodiscard]] unsigned int active()`
    - Get the id of the current active pane

* `[[nodiscard]] Pos get_size()`
    - Get the dimensions of the current active pane

* `void set_content(T)`
* `void set_pane_background(const Color&)`

* `void resize(const Pos& new_size)`
    - Set the dimensions of the current pane

* `void move_cursor_up(int count)`
* `void move_cursor_left(int count)`
* `void move_cursor_right(int count)`
* `void move_cursor_down(int count)`
    - Update the location of the pane's cursor, defaults to 1

* `unsigned int split_vertical(T)`
* `unsigned int split_horizontal(T)`
* `unsigned int split_vertical()`
* `unsigned int split_horizontal()`
    - Split the current pane in two, optionally providing a new content
    to the second created pane

* `void switch_active(unsigned int)`
    - Switch to the pane with the given id if it's a valid leaf node

* `void switch_active()`
    - Switch to the next leaf pane in the BSP tree

* `void close_active()`
* `void draw_all()`

* `void update()`
    - Redraw just the current pane

