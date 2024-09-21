# tenno template library

This library aims to be a replacement for some functionalities of c++ STL,
improving on security and parallelism.

Design goals:
- **exceptions are NEVER thrown**, instead values are returned as `optional` or `result`
- less undefined behaviour, more security
- do not strictly follow silly standards
- thread-safe data structures
- modern-looking and readable c++23 code

Currently implemented:
- [tenno::array<T,N>](./inlude/tenno/array.hpp) (work in progress)
- [tenno::range(int from, int to)](./include/tenno/ranges) (work in progress)

## Building

```bash
cmake -Bbuild
cmake --build build -j 4
```

## Testing
```
cmake -Bbuild -DTENNO_BUILD_TESTS=ON
cmake --build build -j 4
./build/tests --help
```
The library uses [valFuzz](https://github.com/San7o/valFuzz) for testing
```c++
./build/tests              # run tests
./build/tests --fuzz       # run fuzzer
./build/tests --benchmark  # run benchmarks
```

## Documentation

The library uses doxygen for documentation, to build the html documentation run:
```
make docs
```
