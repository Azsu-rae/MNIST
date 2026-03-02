import math

primes = [i for i in range(0, 1000)]

primes.remove(4)

if 4 in primes:
    primes.remove(4)

for i in range(2, 0):
    print("I seem to be doing something")

print(int(math.sqrt(4)))
