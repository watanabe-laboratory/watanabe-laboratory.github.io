# shuffle.py
# shuffle the data

import random, sys
n = int(sys.argv[1]) # the number of samples we want to shuffle
data = []            # list of samples

for i in range(n):
	d = input()
	data.append(d)

random.shuffle(data) 

for i in range(n):
	print(data[i])
	

