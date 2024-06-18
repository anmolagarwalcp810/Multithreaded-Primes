# Multithreaded-Primes
Contains both naive and load-balanced implementation of sieve of eratosthenes.
1. **Naive Approach**: Divides the domain `2...N` into t partitions and assigns each partition to one of the t threads.
2. **Load-Balanced**: each new thread takes up a free number whether it's a prime or not.

[`readme.pdf`](./readme.pdf) contains detailed description of each implementation along with graph plots comparing both naive and load-balanced approach across different number of threads and chunk sizes.

## How to run
```bash
make N=<some_number> t=<num_threads>
```
Program `main.c` launches both sequential (`1.c`) and parallel (`2.c`) programs and compares the compute time of performing seive of eratosthenese over array of size N.
