# select.py
# select instances that are not consistent with the condition

import sys
data = []
n = int(sys.argv[1])  # get the number of instances

for i in range(n):
   d = input()
   data.append(d)

pos = 0
nn = 0  
np = 0  
j = 0

while j < n:
	a = data[j]
	cond = (a[4] == 't')

	if a[0] == 'p':
		pos = pos + 1

	if not cond:
		print(data[j])
		nn = nn + 1
		if a[0] == 'p':
			np = np +1
	j = j + 1

# output
print("*** select result ***\n")
print("basic stat: num. = %d, pos.num. = %d\n" % (n, pos))
print("selected examples: num. = %d, pos.num. = %d\n" % (nn, np))

