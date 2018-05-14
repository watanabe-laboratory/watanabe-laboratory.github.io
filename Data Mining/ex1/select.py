# select.py
# search the samples which are not consistent with the condition

import sys
data = []
n = int(sys.argv[1])

for i in range(n):
   d = input()
   data.append(d)

pos = 0 # the number of the positive samples
nn = 0 # the number of the samples which are not consistent with the condition
np = 0 # the number of the positive samples which are not consistent with the condition 
j = 0

while j < n:
	a = data[j]
	cond = (a[4] == 't') # the condition

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

