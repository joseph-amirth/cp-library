#!/bin/bash

for file in $@; do
    cp template.cpp $file.cpp
done
