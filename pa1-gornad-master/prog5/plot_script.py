import sys
import matplotlib.pyplot as plt
# rootdir = sys.argv[1]
path = sys.argv[1]
rootdir = sys.argv[1] + '/../'
f1 = open(path,'r')

lines = f1.readlines()

y1 = []

for line in lines:
    for i in range (0,1000):
        y1.append(line)
	# y2.append(string[1])
	# print string[0],' ',string[1]

x = []
count = 1
for items in y1:
	x.append(count)
	count += 1
# line 1 points
# plotting the line 1 points
line1, = plt.plot(x, y1, label = "Memory")

# line 2 points
# plotting the line 2 points
# plt.plot(x, y2, label = "Line 2")

# naming the x axis
plt.xlabel('x-axis -> Time')
# naming the y axis
plt.ylabel('y - axis -> Memory in KB')
# giving a title to my graph
plt.title('Total Memory Consumption')

# show a legend on the plot
# line2, = plt.plot([3,2,1], label="Memory Consumption in KB", linewidth=1)
plt.legend(handles=[line1], loc=1)

# function to show the plot
# plt.show()

plt.savefig('memory_utilization.jpg')
plt.show()
