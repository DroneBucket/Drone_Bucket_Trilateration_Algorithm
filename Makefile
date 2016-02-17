all: trilateration

main: trilateration.o
	gcc trilateration.o -lm -o trilateration
	@echo Compilation finie.

clean:
	rm -f trilateration *.o

%.o: %.c
	gcc -c -Wall -ansi $<