#!/bin/bash

for file in $(ls -p | grep -v /); do
    if [[ $file != "template.cpp" ]]; then
        rm $file
    fi
done
