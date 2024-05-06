CC=gcc --std=c99 -g -D_POSIX_C_SOURCE

all: smallsh

smallsh: globals.o command.o functions.o exec.o signals.o main.o
	$(CC) globals.o command.o functions.o exec.o signals.o main.o -o smallsh

main.o: main.c functions.h
	$(CC) -c main.c

functions.o: functions.c functions.h
	$(CC) -c functions.c

exec.o: exec.c exec.h
	$(CC) -c exec.c

signals.o: signals.c signals.h
	$(CC) -c signals.c

command.o: command.c command.h
	$(CC) -c command.c

globals.o: globals.c globals.h
	$(CC) -c globals.c

clean:
	rm -f *.out *.o smallsh