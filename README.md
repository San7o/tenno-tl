# tenno template library

Tenno is a library inspired by the C++23 standard library, compatible
with C++17 language onwards. It aims to provide constexpr and thread-safe containers with security in mind. You can expect
most of the apis to be similar to the standard library.

## Design goals
- **exceptions are NEVER thrown**, instead values are returned as `optional` or `expected`
- **constexpr** functions and data structures
- thread-safe data structures
- modern-looking and readable c++ code
- performance

The library officially supports all GCC compilers starting from gcc-8.5.0.
Constexpr functions are guaranteed to work from c++20 onward.

## Currently implemented
The library aims to provide thread safe / constexpr containers and
algorithms that are not provided by the standard library, currently
the library provides the following features:
- [tenno::array<T,N>](./include/tenno/array.hpp)
- [tenno::expected<T,E>](./include/tenno/expected.hpp)
- [tenno::range\<T>](./include/tenno/ranges.hpp)
- [tenno::error](./include/tenno/error.hpp)
- [tenno::atomic\<T>](./include/tenno/atomic.hpp)
- [tenno::mutex](./include/tenno/mutex.hpp)
- [tenno::lock_guard\<T>](./include/tenno/mutex.hpp)
- [tenno::optional\<T>](./include/tenno/optional.hpp)
- [tenno::shared_ptr\<T>](./include/tenno/memory.hpp)
- [tenno::copy<It1,It2,F>](./include/tenno/algorithm.hpp)
- [tenno::for_each<It1,It2,F>](./include/tenno/algorithm.hpp)
- [tenno::accumulate<It1,It2,T>](./include/tenno/algorithm.hpp)
- [tenno::swap\<T>](./include/tenno/algorithm.hpp)
- [tenno::move\<T>](./include/tenno/utility.hpp)
- [tenno::make_shared\<T, Deleter, Alloc>](./include/tenno/memory.hpp)
- [tenno::allocate_shared\<T, Alloc>](./include/tenno/memory)
- [tenno::unique_ptr\<T>](./include/tenno/unique_ptr.hpp)
- [tenno::make_unique<\T>](./include/tenno/memory.hpp)
- [tenno::jthread](./include/tenno/thread.hpp)
- [tenno::weak_ptr\<T>](./include/tenno/memory.hpp)
- [tenno::allocator\<T>](./include/tenno/memory.hpp)
- [tenno::default_delete\<T>](./include/tenno/memory.hpp)
- [tenno::vector\<T>](./include/tenno/vector.hpp)
- [tenno::reference_wrapper](./include/tenno/functional.hpp)
- tenno::deque: TODO
- tenno::stack: TODO
- tenno::map: TODO
- tenno::unordered_map: TODO
- tenno::mdspan: TODO
- tenno::thread pool: TODO

## Building

### Using bazel
```bash
bazel build //:libtenno
```

### Using Cmake
Build as a library:
```bash
cmake -Bbuild -DTENNO_BUILD_SHARED=ON -DTENNO_BUILD_OPTIMIZED=ON
```
Note that tests are enabled by default.

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

## Differences with STL

## array

`tenno::array<T,N>.at(n)` returns `expected<T,E>` with either the value
or a `tenno::error` with the error `out_of_range` if the range specified
is bigger than the size of the array.

## vector

- `tenno::vector<T>.at(n)` returns `expected<T,E>`
- `tenno::vector<T>.front()` return `expected<const T&,E>`
- `tenno::vector<T>.back()` returns `expected<const T&,E>`
- `tenno::vector<T>.operator[]` returns `expected<T,E>`
- all iterator access methods return `expected<T,E>`
- iterator is divided into `iterator` and `iterator_mut`

## functional

- `std::reference_wrapper\<T>.get()` has beed renamed to `std::reference_wrapper\<T>.ref()`
