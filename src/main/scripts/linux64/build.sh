#!/usr/bin/env bash

VERSION=${VERSION%"-SNAPSHOT"}

cmake . \
-DCMAKE_MODULE_PATH=. \
-DCMAKE_BUILD_TYPE=Release \
-DLIBRARY_OUTPUT_PATH="./bin/linux64" \

make
r1=$?

exit ${r1}
