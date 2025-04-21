#!/bin/bash

FILE=${1:-main}
[[ build/$FILE -nt $FILE.cpp ]] || ./scripts/build.sh $FILE
exec build/$FILE
