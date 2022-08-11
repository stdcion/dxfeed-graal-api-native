## About

The project is a wrapper over dxFeed Graal API library (Java), which is built using GraalVM (native-image) technology.

## Requirement

1) CMake 3.20+
2) Compiler (C++11, C11)

## Build

Run script

```text
build_linux.sh
build_mac.sh
build_win.cmd
```

## Structure

1) `bin` - artifact output directory.
2) `build` - build directory.
3) `cmake` - contains custom CMake modules.
4) `extern` - extern source.
5) `src` - source directory.
6) `test` - test directory.