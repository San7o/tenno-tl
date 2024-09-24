# run a benchmark

```bash
./build/tests --run-one-benchmark "tenno::array()" --num-iterations 10000000 
```

Use `perf` for profiling
```bash
perf stat -d ./build/tests --benchmark
```
