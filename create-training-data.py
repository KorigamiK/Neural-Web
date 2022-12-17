import random

"""
This script creates a simple XOR prediction training data set for the neural network.
save the output set to a file called training-data.txt in the test build directory.

run: `python create-training-data.py > training-data.txt`
"""

print("topology: 2 4 1")
for i in range(2000):
    n1 = random.randint(0, 1)
    n2 = random.randint(0, 1)
    print("in:", f"{n1}.0 {n2}.0\nout: {n1 ^ n2}")
