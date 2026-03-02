import math

primes = [i for i in range(0, 1000)]

primes.remove(4)

if 4 in primes:
    primes.remove(4)

print(int(math.sqrt(4)))

