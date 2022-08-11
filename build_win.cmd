cmake --preset="conf-win-vs" -DCMAKE_BUILD_TYPE=Release .
cmake --build --preset="build-win-vs" --config=Release
ctest --test-dir build/conf-win-vs -C Release --extra-verbose
