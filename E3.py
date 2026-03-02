import time
import random

random.seed(42)
l_A = [random.randint(0, 500_000) for _ in range(0, 100_000)]
l_B = [random.randint(0, 500_000) for _ in range(0, 100_000)]

start = time.time()
intersection = []
for a in l_A:
    for b in l_B:
        if a == b:
            intersection.append(a)
            break
end = time.time()
non_optimal = end - start

# for the optimal solution, using a set (or some data structure that uses hashing for O(1) access)

start = time.time()
intersection = []
s_B = set(l_b)
# l_A = set(l_A) optionally, to avoid duplicates if they don't matter
for a in l_A:
    if a in s_B:
        intersection.append(a)
end = time.time()
non_optimal = end - start

print(f"Intersection size: {len(intersection)}")
print(f"Elapsed time (python): {non_optimal}")

# -------- OUTPUT --------
# Intersection size: 17889
# Elapsed time (python): 236.7335638999939
