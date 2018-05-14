# test.py
# test the accuracy of the condition

import sys
data = []
n = int(sys.argv[1])

for i in range(n):
   d = input()
   data.append(d)

pos = 0 # the number of the positive samples
err = 0  # the number of the errors
fpos = 0  # the number of the negative samples which are postive according to the condition
fneg = 0 # the number of the postive samples which are negative according to the condition
j = 0

while j < n:
   a = data[j]
   cond = (a[4] == 't') # the condition

   if a[0] == 'p':
      pos = pos + 1

   if cond and a[0] != 'p':
      fpos = fpos + 1 # count the number of false postive samples
      err = err + 1 # count the number of errors
   if (not cond) and a[0] != 'e':
      fneg = fneg + 1 # count the number of false negative samples
      err = err + 1 # count the number of errors
   j = j + 1
   
# output
print("***test result***\n")
print("aasic stat: num. = %d, pos.num. = %d (%3.2f)\n" % (n, pos, pos/n))

neg = n - pos
acc = n - err
print("    accuracy:       %d (%3.2f)\n" % (acc, acc/n))
print("    false positive: %d (%3.2f)\n" % (fpos, fpos / neg))
print("    false negative: %d (%3.2f)\n" % (fneg, fneg / pos))


