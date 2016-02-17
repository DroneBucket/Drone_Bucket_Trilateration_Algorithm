all: main

main: main.o
	gcc main.o -lm -o main
	@echo Compilation finie.

clean:
	rm -f main *.o

%.o: %.c
	gcc -c -Wall -ansi $<