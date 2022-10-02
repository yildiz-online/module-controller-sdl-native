#!/usr/bin/env bash

VERSION=${VERSION%"-SNAPSHOT"}

cmake . \
-DCMAKE_MODULE_PATH=. \
-DCMAKE_BUILD_TYPE=Release \
-DLIBRARY_OUTPUT_PATH="/src/bin" \
-DCMAKE_TOOLCHAIN_FILE=mingw-toolchain.cmake

make
r1=$?

exit ${r1}
