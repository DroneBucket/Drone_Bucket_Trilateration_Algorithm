all: trilateration

main: trilateration.o
	gcc trilateration.o -lm -o trilateration
	@echo Compilation finie.

<<<<<<< HEAD
=======
clean:
	rm -f trilateration *.o

>>>>>>> branch 'master' of https://github.com/DroneBucket/Drone_Bucket_Trilateration_Algorithm.git
%.o: %.c
	gcc -c -Wall -ansi $<

clean:
	rm -f main *.o