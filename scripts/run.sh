#!/bin/bash

FILE=${1:-main}
./scripts/build.sh $FILE
exec build/$FILE
