# count.py
# count the number of the samples we need

import sys
data = []
n = int(sys.argv[1])

for i in range(n):
   d = input()
   data.append(d)

i = 4 # the i-th attribute we want to count
v = 't' # the value of the i-th attribute we want to count
pos = 0 # the number of the positive samples	
npos = 0 # the number of the positive samples under the condition
nneg = 0 # the number of the nagative samples under the condition
j = 0

while j < n:
	a = data[j]

	if a[0] == 'p':
		pos = pos + 1

	if a[i] == v: # the condition
		if a[0] == 'p':
			npos = npos + 1
		else:
			nneg = nneg + 1
	j = j + 1

print("*** count result ***\n")
print("basic stat: num. = %s, pos.num. = %d\n" % (n, pos))
print("    a[%d] == %s:\n    num = %d, pos = %d, neg= %d, accuracy = %3.2f\n" % (i, v, npos + nneg, npos, nneg, npos/(npos+nneg)))
