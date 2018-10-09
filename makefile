CC = gcc
CFLAGS = -g -Wall

main : main.o printStats.o 
	${CC} ${CFLAGS} -o schedulerSim main.o 
main.o : printStats.o a2.h

printStats.o : a2.h



.PHONY : clean


clean: 
	rm -f *.o core schedulerSim