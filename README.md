
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
