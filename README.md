
```bash
mpirun -np 4 ./hello_mpi
```

If this doesn't aligns with your physical cores, allow oversubscription:

```bash
mpirun --oversubscribe -np 4 ./hello_mpi
```

This tells OpenMPI:
“Run more processes than cores; I accept the performance trade-off.”

If you want to see what OpenMPI thinks your system has:

```bash
lscpu
```

Look at:

* `CPU(s)` → total logical processors
* `Core(s) per socket` → physical cores

* A **slot ≈ one process execution capacity**
* MPI tries to map **1 process per core** by default
* Oversubscription = more processes than cores → context switching

# TODO
  5. Per-byte file reads. readbyte is called 60000 × 784 ≈ 47 M times. stdio buffers help, but you're still paying ~47 M function calls + stride

  checks. The whole image block is contiguous on disk — read it in one shot:

  fread(dataset->content, 1, size * rows * cols, fi);
  fread(dataset->labels,  1, size, fl);

  That replaces the entire for k / for i / for j loop and is dramatically faster.


  6. Function-pointer dispatch per pixel. img.set(img, i, j, byte) goes through an indirect call, a bounds check, and a stride mul — 47 M times.
  Even after switching to bulk fread you'll want direct buffer access (or inline'd accessors) when training the MLP. Method dispatch per pixel will
   dominate over the actual matmul work.

  Design observations (not bugs)

  7. Image is heavy. 4 function pointers (32 B) + Shape (16) + 2 pointers (16) = ~64 B per Image, returned by value every dataset->index(). Fine
  for printing one digit, but if you call dataset->index(...) inside training loops you're churning the cache for no reason. Two cleaner options:

  - Keep Dataset, drop Image as a struct. Just expose dataset_pixel(d, k, r, c) and dataset_label(d, k) (or inline accessors). Less elegant but
  matches what training code actually needs: flat array + index math.
  - Make Image a plain view without function pointers: just { shape, lbl_ptr, content_ptr }. Move get/set/print to free functions taking Image*.
  Same ergonomics, no per-call dispatch, no per-Image function-pointer storage.

  8. img.set(img, ...) passes Image by value. It works because content is a pointer (so writes still hit dataset memory), but it's surprising —
  _label, _set, _get cannot mutate the caller's Image. Conventional C-OOP takes Image* so methods can update state. Currently it doesn't matter
  because Image fields aren't mutated, but worth knowing if you start adding state.

  9. _get / _set bounds-check on every access. Useful while debugging; you'll want to compile them out (or skip them in tight loops) when you start
   training. Consider an #ifdef NDEBUG guard.
