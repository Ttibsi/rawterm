#!/bin/bash

set -xe

mkdir -p ./build/
g++ -std=c++17 ./main.cpp -o ./build/main
g++ -std=c++17 ./escape.cpp -o ./build/escape
