# MemoryAllocatorCPP

A custom C++ dynamic memory allocator built for UT Austin CS371p. The project explores how `malloc`/`free`–style allocators work under the hood: block metadata, free‑list management, alignment, splitting/coalescing, and fragmentation trade‑offs.

## Why this exists

Standard allocators are black boxes. Re‑implementing one forces you to reason about:

- How big should metadata be?  
- When do you split a block vs. leave internal fragmentation?  
- How do you find a “good enough” free block fast?  
- How do you safely coalesce adjacent frees without corrupting the heap?

## Key Features

- **Explicit free list** with doubly linked blocks  
- **First-fit (or configurable) placement** policy  
- **Block splitting & boundary-tag coalescing**  
- **8/16‑byte alignment guarantees** (configurable)  
- **Guard checks & asserts** in debug builds  
- **Unit & micro-benchmark tests** (GoogleTest / custom harness)

## High-Level Design

1. **Heap initialization** – request an initial chunk via `sbrk`/`mmap` (or a simulated buffer in tests).  
2. **Allocation path**  
   - Align request size → add header/footer → search free list  
   - If found: remove or split block, return payload pointer  
   - If not: extend heap, create new block  
3. **Free path**  
   - Mark block free, insert into free list  
   - Attempt coalesce with prev/next using boundary tags  
4. **Realloc** (if implemented)  
   - If room to expand in place, reuse  
   - Otherwise allocate new, memcpy, free old

## Public API (simplified)

```cpp
void* mm_malloc(size_t size);
void  mm_free(void* ptr);
void* mm_realloc(void* ptr, size_t size);
```

> Update these names/signatures if your repo differs.

## Building & Running

```bash
# CMake
cmake -S . -B build
cmake --build build --config Release
./build/bin/allocator_demo    # if provided
```

_or_

```bash
make
./allocator_demo
```

## Testing

```bash
# With GoogleTest
cmake -S . -B build -DENABLE_TESTS=ON
cmake --build build
ctest --test-dir build

# or custom
make test
```

Benchmarks (if included):

```bash
./build/bin/bench_alloc --iterations 100000 --pattern random
```

## Repository Layout

```
.
├── src/                # allocator implementation
├── tests/              # unit & perf tests
├── demo/               # sample usage
├── CMakeLists.txt / Makefile
└── README.md
```

(Adjust to match your actual tree.)

## Usage Example

```cpp
#include "allocator.hpp"

int main() {
    mm_init();
    int* arr = static_cast<int*>(mm_malloc(100 * sizeof(int)));
    // ... use arr ...
    mm_free(arr);
    mm_shutdown();
    return 0;
}
```

## Collaborators & Roles

- **Katherine Liang** — primary implementation (allocator core, integration), initial design write‑up  
- **Cash Belknap** — debugging, unit/benchmark test development, performance analysis, API polish

## Possible Extensions

- Segregated free lists (size classes)  
- Best-fit / next-fit comparison  
- Thread-safety (locks or lock-free lists)  
- Red zones / canaries for overflow detection  
- Statistical logging (external vs. internal fragmentation over time)
