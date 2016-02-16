all: main

main: main.o
	gcc main.o -o main -lm
	@echo Compilation finie.

%.o: %.c
	gcc -c -Wall -ansi $<

clean:
	rm -f main *.o