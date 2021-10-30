import random
n = int(input())
m = int(input())

with open('in.txt', 'w') as f:
	f.write(str(n) + ' ' + str(random.randint(1, m+1)) + '\n')
	for i in range(n):
		s = "{}\n".format(random.randint(1, 50))
		f.write(s)
