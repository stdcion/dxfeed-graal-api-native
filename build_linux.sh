#!/bin/sh
cmake --preset="conf-linux" -DCMAKE_BUILD_TYPE=Release .
cmake --build --preset="build-linux" --config=Release
ctest --test-dir build/conf-linux -C Release --extra-verbose
