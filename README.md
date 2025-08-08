# Profiling matrix multiplication

This repo contains two implementations for square matrix-matrix multiplication, both written in C:

- `naive_multiply`, a "naïve" implementation that simply iterates and calculates over all entries of $C$, the resulting matrix.
- `tiled_multiply`, an implementation that attempts to be "cache-aware", by partitioning the input matrices into square tiles.

Both implementations are profiled using [gprof](https://ftp.gnu.org/old-gnu/Manuals/gprof-2.9.1/html_mono/gprof.html), which provides simple timing measurements. The following parameters are considered:

- Tile size.
- Input matrix size, $n$.
- Compiler optimization flags, comparing `gcc`'s `-O0` and `-O2`.

## Results

`results/x86/` contains all `gprof` flat profile output files, as well as simple plots visualizing the results. 

Based on the profiling output:

- The simple tiling approach used resulted in a moderate reduction in execution time, compared to the naïve approach. Tiling and other memory-aware optimizations can have a greater impact, but they require careful consideration of system architecture and memory access patterns.
- Enabling compiler optimization flags provided a noticeable increase in performance. 
- The profiling output is limited, and can vary. `gprof` does not measure relevant PMU hardware events, such as cache misses or branch mispredictions. On some consecutive runs with the same parameters, the values of `gprof`'s `self s/call` field differed by as much as 0.5 seconds.

## Future improvements

Based on the results obtained, there are several concerns that could be addressed to improve both program performance and the profiling method:

- Use tools such as `perf` and `cachegrind`, to gain more insight into the memory access patterns of the program.
- Use a layered tiling approach with 
- General compiler optimization flags like `-O2` are useful, but better performance could be achieved by selecting individual flags. It would also be nice to inspect the generated binaries to see if they use the available vectorized / fast math instructions available on the target.
- Try different architectures and testing setups. `WSL2` on Windows was used because it's on hand, but it does not support profiling all hardware events, and the testing environment might not reflect other relevant conditions. It would also be interesting to test on RISC architectures (ARM, RISC-V) and on Linux.

## Method

### Get started

TODO

### Matrix multiplication

If $A$ and $B$ are two square matrices with dimensions $n$ x $n$, their product is another $n$ x $n$ matrix $C$:
$$C = AB$$
Such that every entry $c_{ij}$ in $C$ is given by
$$c_{ij}=\sum_{k=0}^{n} a_{ik}b_{kj}$$
Matrix multiplication is a common operation and can be found in a wide range of applications. As such, there is ongoing research into improving the performance of matrix multiplication in computers, especially for large $n$. Matrix multiplication can be optimized in different ways:

1. Algorithm choice: a simple iterative implementation has $O(n^3)$ complexity, but other algorithms offer better performance with an exponent smaller than 3, such as Strassen's algorithm. However, not all matrix multiplication algorithms are practical .
2. Parallelization: if the algorithm follows a divide-and-conquer approach, it could also be parallelized.
3. Memory access patterns: most of the current CPUs have a layered memory architecture, with one or more levels of faster-than-RAM cache. Ensuring that the implementation takes advantage of the available cache helps ensure execution time is dominated by floating point operations, instead of memory waits.
[A brief overview of matrix multiplication](https://en.wikipedia.org/wiki/Matrix_multiplication)

### Optimization strategies

#### Tiling

It is possible to partition the input matrices into submatrices ("tiles" or "blocks"). The choice of tile size then becomes important, and depends on the available cache (hierarchy, size, associativity, policies, etc.).

#### Compiler flags

It is usually desired to ensure the binary uses  The following specific GCC flags control vectorization optimization [2](https://gcc.gnu.org/onlinedocs/gcc/Optimize-Options.html):

- `-ftree-vectorize`: enables vectorization on trees, also enables `-ftree-loop-vectorize` and `-ftree-slp-vectorize`.
- `-fvect-cost-model` adjusts the decision-making model used by the compiler for vectorization. It can assume the vectorized code path is profitable with `unlimited`, or it can evaluate its cost using a runtime check.

```
gcc -ftree-vectorize -fvect-cost-model=very-cheap -msse2 -mfpmath=sse -mtune=generic
```

At least in the test environment, the previous flags require careful C code structure, as well as other GCC optimization flags, in order to provide noticeable improvements. During profiling, providing these flags by themselves did not result in a noticeable performance increase.

### Measuring performance

#### Constraints

- $A$, $B$, $C$ are all square matrices of size $n$ x $n$.
- $50 \le n \le  500$ (for `-O2`, tests were done with $50 \le n \le  1000$)
- All matrix entries are 32-bit single-precision floating point numbers.

#### Environment

- CPU: Intel(R) Core(TM) i7-8750H CPU @ 2.20GHz, 6 cores, 6 x 32 KB L1D, 6 x 256 KB L2
- OS: Ubuntu 24.04.1 LTS, running on WSL2 Windows 10

#### GNU Profiler

Use GNU profiler, `gprof`, to profile the example matrix multiplication. Profiling includes the following steps:

1. Compile and link with profiling enabled, passing the `-pg` and `-g` flags to GCC. The `Makefile` takes care of this.
2. Execute the program to generate profiling data files.
3. Run `gprof` to analyze profiling data, generating a flat profile, call graph, and/or annotated source.

Because the profiling analysis is focused on matrix multiplication performance, given that `naive_multiply()` and `tiled_multiply()` don't invoke more functions, the call graph data file isn't relevant and is not generated.

Execute the program normally, then use `gprof`to generate the profiling data files:

```bash
gprof -pnaive_multiply -ptiled_multiply -b mm_naive.exe > gprof_output.txt
```

The `-p` flag makes `gprof` generate only a flat profiling file. `-p` includes a *symspec*, to only include the functions relevant for performance. The `-b` option produces brief output.

In the `Makefile`, the compile command includes the `-no-pie` flag. This is a workaround for a [MYSYS bug](https://github.com/msys2/MINGW-packages/issues/8503) that produces an empty flat profile. It is only required for Windows MYSYS.

`gprof` is useful for an initial analysis, with the goal to identify costly functions and to sample the average time spent in each invocation. However, it does not reveal much more about performance.

## Useful resources

- [Amitabha Banerjee, Optimizing matrix multiplication](https://www.cs.purdue.edu/homes/grr/cs250/lab6-cache/optimizingMatrixMultiplication.pdf)
- [Vectorization optimization in GCC](https://developers.redhat.com/articles/2023/12/08/vectorization-optimization-gcc#)
- [David Bindel, Lecture 2: Tiling matrix-matrix multiply, code tuning](https://www.cs.cornell.edu/~bindel/class/cs5220-s10/slides/lec03.pdf)
- [Optimizing Cache Performance in Matrix Multiplication](https://sites.cs.ucsb.edu/~tyang/class/240a17/slides/Cache3.pdf)
- [Anatomy of high-performance matrix multiplication](https://dl.acm.org/doi/10.1145/1356052.1356053)