import random

for i in range(2000):
    n1 = random.randint(0, 1)
    n2 = random.randint(0, 1)
    print("in:", f"{n1}.0 {n2}.0\nout: {n1 ^ n2}")
