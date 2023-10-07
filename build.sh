#!/bin/bash

set -xe

mkdir -p ./build/
g++ -std=c++20  -ggdb ./main.cpp -o ./build/main
g++ -std=c++20 ./escape.cpp -o ./build/escape
