# count.py
# count the number of the instances with a certain attribute value

import sys
data = []
n = int(sys.argv[1])  # get the number of instances

for i in range(n):
   d = input()
   data.append(d)

i = 4    # the i-th attribute we want to count
v = 't'  # the value of the i-th attribute we want to count
pos = 0  # the number of the positive instances
npos = 0 # the number of the positive instances under the condition
nneg = 0 # the number of the nagative instances under the condition
j = 0

while j < n:
	a = data[j]

	if a[0] == 'p':
		pos = pos + 1

	if a[i] == v:
		if a[0] == 'p':
			npos = npos + 1
		else:
			nneg = nneg + 1
	j = j + 1

print("*** count result ***\n")
print("basic stat: num. = %s, pos.num. = %d\n" % (n, pos))
print("    a[%d] == %s:\n    num = %d, pos = %d, neg= %d, accuracy = %3.2f\n" % (i, v, npos + nneg, npos, nneg, npos/(npos+nneg)))
