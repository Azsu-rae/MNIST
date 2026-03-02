import time

n = 10000000
total = 0

start = time.time()

for i in range(1, n + 1):
    total += i

end = time.time()
non_optimal = end - start

start = time.time()
total = n * (n + 1) // 2
end = time.time()
optimal = end - start

print(f"optimal = {optimal}")
print(f"non_optimal = {non_optimal}")
print(f"improved over = {non_optimal / optimal} times")
