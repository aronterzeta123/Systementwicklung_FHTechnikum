all: program sensor
program: program.c 
	gcc -Wall --pedantic -Wextra program.c -o program
sensor: sensor.c
	gcc -Wall --pedantic -Wextra sensor.c -o sensor
clean:
	rm -rf *~ *.o program sensor
