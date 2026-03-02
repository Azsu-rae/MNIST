import time

def is_prime_naive(n):
    if n < 2:
        return False
    for i in range(2, n):
        if n % i == 0:
            return False
    return True


def is_prime(n):
    if n < 2:
        return False
    for i in range(2, int(sqrt(n))):
        if n % i == 0:
            return False
    return True


N = 100000
start = time.time()
primes = [x for x in range(2, N + 1) if is_prime_naive(x)]
end = time.time()
non_optimal = end - start

start = time.time()
end = time.time()

print(f"Number of primes found: {len(primes)}")
print(f"Elapsed time (python): {end - start:.6f} seconds")
