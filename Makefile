all: main

main: main.o
	gcc main.o -lm -o main
	@echo Compilation finie.

%.o: %.c
	gcc -c -Wall -ansi $<

clean:
	rm -f main *.o