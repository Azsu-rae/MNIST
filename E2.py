import time
import math

def is_prime_naive(n):
    if n < 2:
        return False
    for i in range(2, n):
        if n % i == 0:
            return False
    return True


def is_prime_math(n):
    if n < 2:
        return False
    for i in range(2, int(math.sqrt(n))):
        if n % i == 0:
            return False
    return True


N = 100000
numbers = [x for x in range(0, N+1)]
start = time.time()
non_opt_primes = [x for x in numbers if is_prime_naive(x)]
end = time.time()
non_optimal = end - start

start = time.time()
for num in numbers:
    if num != 0 and is_prime_math(num):
        factor = 2
        multiple = num * factor
        while multiple < N+1:
            numbers[multiple] = 0
            factor += 1
            multiple = num * factor
end = time.time()
optimal = end - start
opt_primes = [x for x in numbers if x >= 2 and x != 0]

print(f"Number of primes found:")
print(f"optimal: {len(opt_primes)}")
print(f"non_optimal: {len(non_opt_primes)}")

print(f"Elapsed time (python):")
print(f"optimal: {optimal}")
print(f"non_optimal: {non_optimal}")
print(f"ratio: {non_optimal / optimal}")
