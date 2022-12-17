import random

for i in range(2000):
    n1 = random.randint(0, 1)
    n2 = random.randint(0, 1)
    print("in:", f"{n1}.0 {n2}.0\nout: {n1 ^ n2}")

# # Path: train.py
# import neat
# import os
# import visualize
#
# # 1. Load the training data
# training_data = []
# with open('training_data.txt', 'r') as f:
#     for line in f:
#         training_data.append(line.strip().split(' '))
