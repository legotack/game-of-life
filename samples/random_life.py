import sys
import random

filename = ""
if len(sys.argv) > 1:
	filename = sys.argv[1]
else:
	filename = "random-life.txt"

num_cells = int(input("How many cells: "))

file = open(filename, 'w')
for i in range(0, num_cells):
	file.write("(" + str(random.randint(-2000000000000, 2000000000000)) + ", " + str(random.randint(-2000000000000, 2000000000000)) + ")\n")

file.close()