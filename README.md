# YALG
Yet another implementatoion of CompSci algorithms. Must be suitable for competitive programming

NB!: You must clone the project with `git clone --recurse-submodules ...` to fetch gooletest submodule

Requires `cmake` version 3.8+ and `doxygen` to build

How to build:

```shell
mkdir build
cd build
cmake ..
cmake --build . -- -j
# run all tests
cmake --build . --target test
```
