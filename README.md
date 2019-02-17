# YALG
Yet another implementatoion of CompSci algorithms. Must be suitable for competitive programming

NB!: You must clone the project with `git clone --recurse-submodules ...` to fetch gooletest submodule

Requires `cmake` version 3.8+ and `doxygen` to build

How to build:

```shell
mkdir build
cd build
# cmake3 .. # if you're on RedHat
# use Release coonfig for full optimization
cmake -DCMAKE_BUILD_TYPE=Release ..
cmake --build . -- -j
# run all tests
cmake --build . --target test
# heap_test may fail performance test on debug config
```
