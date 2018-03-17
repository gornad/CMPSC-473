# HEADERS = program.h headers.h

default: prog3

# program.o: program.c $(HEADERS)
#     gcc -c program.c -o program.o

prog3: prog3.c
	gcc -g -std=c99 prog3.c -o prog3 -lm

clean:
	-rm -f prog3