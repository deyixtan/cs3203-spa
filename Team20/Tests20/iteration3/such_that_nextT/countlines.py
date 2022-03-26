import sys

count = 1
lines = []

file1 = sys.argv[1]
file2 = sys.argv[2]

with open(file1, "r") as f:
	lines = f.readlines()

with open(file2, "w") as f:
	for n in lines:
		if(n.find("procedure") == -1 and n.find("}") == -1 and n != "\n"):
			f.write(str(count) + " " + n)
			count += 1
		else:
			f.write(n)
