#!/bin/bash

FILE=${1:-main}
make build/$FILE && exec build/$FILE
