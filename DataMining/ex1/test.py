# test.py
# test the accuracy of the condition

import sys
data = []            
n = int(sys.argv[1])  # get the number of instances

for i in range(n):
   d = input()
   data.append(d)

pos = 0  # the number of the positive instances
err = 0  # the number of the errors, i.e., misclassfied instances
fpos = 0 # the number of the negative but misclassified as positive instances
fneg = 0 # the number of the postive but misclassified as negative instances
j = 0

while j < n:
   a = data[j]

   ##### condition is given here #####
   cond = (a[4] == 't') 
   ##### condition is given here #####

   if a[0] == 'p':
      pos = pos + 1

   if cond and a[0] != 'p':
      fpos = fpos + 1 # count the number of false postive instances
      err = err + 1   # count the number of errors
   if (not cond) and a[0] != 'e':
      fneg = fneg + 1 # count the number of false negative instances
      err = err + 1   # count the number of errors
   j = j + 1
   
# output
print("***test result***\n")
print("aasic stat: num. = %d, pos.num. = %d (%3.2f)\n" % (n, pos, pos/n))

neg = n - pos
acc = n - err
print("    accuracy:       %d (%3.2f)\n" % (acc, acc/n))
print("    false positive: %d (%3.2f)\n" % (fpos, fpos / neg))
print("    false negative: %d (%3.2f)\n" % (fneg, fneg / pos))


