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


## Collaborators & Roles

- **Katherine Liang** — primary implementation (allocator core, integration), initial design write‑up  
- **Cash Belknap** — debugging, unit/benchmark test development, performance analysis, API polish

## Possible Extensions

- Segregated free lists (size classes)  
- Best-fit / next-fit comparison  
- Thread-safety (locks or lock-free lists)  
- Red zones / canaries for overflow detection  
- Statistical logging (external vs. internal fragmentation over time)
