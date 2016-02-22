all: trilateration

trilateration: trilateration.o
	gcc trilateration.o -o trilateration -lm
	@echo Compilation finie.

%.o: %.c
	gcc -c -Wall  -std=c99 $<

clean:
	rm -f trilateration *.o