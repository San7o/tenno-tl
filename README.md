# tenno template library

This library aims to be a replacement for some functionalities of the STL for c++23,
improving on security and parallelism. You can expect all the functionalities to behave like STL.

## Design goals
- **exceptions are NEVER thrown**, instead values are returned as `optional` or `expected`
- **constexpr** everywhere It's possible
- less undefined behaviour, more security
- thread-safe data structures
- modern-looking and readable c++23 code
- do not strictly follow silly standards
- optimize for performance

## Currently implemented
- [tenno::array<T,N>](./include/tenno/array.hpp)
- [tenno::expected<T,E>](./include/tenno/expected.hpp)
- [tenno::range\<T>](./include/tenno/ranges.hpp)
- [tenno::error](./include/tenno/error.hpp)
- [tenno::atomic\<T>](./include/tenno/atomic.hpp)
- [tenno::mutex](./include/tenno/mutex.hpp)
- [tenno::lock_guard\<T>](./include/tenno/mutex.hpp)
- [tenno::optional\<T>](./include/tenno/optional.hpp)
- [tenno::shared_ptr\<T>](./include/tenno/shared_ptr.hpp)
- [tenno::copy<It1,It2,F>](./include/tenno/algorithm.hpp)
- [tenno::for_each<It1,It2,F>](./include/tenno/algorithm.hpp)
- [tenno::accumulate<It1,It2,T>](./include/tenno/algorithm.hpp)
- [tenno::swap\<T>](./include/tenno/algorithm.hpp)
- [tenno::move\<T>](./include/tenno/utility.hpp)
- [tenno::make_shared\<T>](./include/tenno/memory.hpp)
- [tenno::allocate_shared](./include/tenno/memory) (TODO: array specialization)
- [tenno::unique_ptr](./include/tenno/unique_ptr.hpp) (TODO: array specialization)
- [tenno::make_unique](./include/tenno/memory.hpp)
- [tenno::jthread](./include/tenno/thread.hpp)
- tenno::weak_ptr: TODO
- tenno::allocator: TODO
- tenno::vector: TODO
- thread pool: TODO

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

# Differences with STL

## array

`tenno::array<T,N>.at(n)` returns `expected<T,E>` with either the value
or a `tenno::error` with the error `out_of_range` if the range specified
is bigger than the size of the array.
