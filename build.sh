#!/bin/bash

set -xe

mkdir -p ./out/
# g++ -std=c++20 -Wall -Wextra ./examples/colors.cpp       -o ./out/colors
# g++ -std=c++20 -Wall -Wextra ./examples/hello_world.cpp  -o ./out/hello_world
# g++ -std=c++20 -Wall -Wextra ./examples/keys.cpp         -o ./out/keys
# g++ -std=c++20 -Wall -Wextra ./examples/raw_escapes.cpp  -o ./out/raw_escapes

# x86_64-w64-mingw32-g++ -std=c++20 -Wall -Wextra -static-libgcc -static-libstdc++ ./examples/colors.cpp       -o ./out/colors
x86_64-w64-mingw32-g++ -std=c++20 -Wall -Wextra -static-libgcc -static-libstdc++ ./examples/hello_world.cpp  -o ./out/hello_world
# x86_64-w64-mingw32-g++ -std=c++20 -Wall -Wextra -static-libgcc -static-libstdc++ ./examples/keys.cpp         -o ./out/keys
# x86_64-w64-mingw32-g++ -std=c++20 -Wall -Wextra -static-libgcc -static-libstdc++ ./examples/raw_escapes.cpp  -o ./out/raw_escapes

