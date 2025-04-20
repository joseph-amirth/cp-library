#!/bin/bash

FILE=${1:-main}
[[ -d build ]] || mkdir build
g++ -std=c++20 -Wall $FILE.cpp -o build/$FILE
