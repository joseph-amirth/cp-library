#!/bin/bash

set +e

FILE=${1:-main}
make build/$FILE && echo "Running..."
exec build/$FILE

set -e
