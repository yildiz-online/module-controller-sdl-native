#!/usr/bin/env bash

VERSION=${VERSION%"-SNAPSHOT"}

cmake . \
-DCMAKE_MODULE_PATH=. \
-DCMAKE_BUILD_TYPE=Release \
-DLIBRARY_OUTPUT_PATH="src/main/scripts/bin" \

make
r1=$?

exit ${r1}
