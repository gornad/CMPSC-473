all: disk_simulation

disk_simulation: disk_simulation.c driver.c disk.c driver.h
	gcc -g -o disk_simulation disk_simulation.c driver.c disk.c driver.h -std=gnu99 -pthread -lrt

clean:
	rm -rf disk_simulation
