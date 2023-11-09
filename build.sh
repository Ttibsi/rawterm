#!/bin/bash

set -xe

mkdir -p ./out/
g++ -std=c++20 -Wall -Wextra ./examples/colors.cpp      -o ./out/colors
g++ -std=c++20 -Wall -Wextra ./examples/keys.cpp        -o ./out/keys
g++ -std=c++20 -Wall -Wextra ./examples/raw_escapes.cpp -o ./out/raw_escapes
