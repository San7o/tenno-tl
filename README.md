# tenno template library

This library aims to be a replacement for some functionalities of c++ STL,
improving on security and parallelism.

Design goals:
- **exceptions are NEVER thrown**, instead values are returned as `optional` or `result`
- less undefined behaviour, more security
- do not strictly follow silly standards
- thread-safe data structures
- modern-looking and readable c++23 code
- **constexpr** everywhere It's possible

Currently implemented:
- [tenno::array<T,N>](./include/tenno/array.hpp)
- [tenno::range<T>](./include/tenno/ranges.hpp)
- [tenno::copy<It1,It2,F>](./include/tenno/algorithm.hpp)
- [tenno::for_each<It1,It2,F>](./include/tenno/algorithm.hpp)
- [tenno::accumulate<It1,It2,T>](./include/tenno/algorithm.hpp)
- [tenno::expected<T,E>](./include/tenno/expected.hpp)
- [tenno::error](./include/tenno/error.hpp)

## Testing

### Using Cmake
```bash
cmake -Bbuild -DTENNO_BUILD_TESTS=ON
cmake --build build -j 4
./build/tests --help
```

### Using meson

```bash
meson setup build
ninja -C build
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
