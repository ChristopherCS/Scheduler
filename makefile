CC = gcc
CFLAGS = -g -Wall -pedantic

main : main.o printStats.o 
	${CC} ${CFLAGS} -o schedulerSim main.o 
main.o : printStats.o a2.h

printStats.o : a2.h

test: main
	./schedulerSim a2.in


.PHONY : clean log


clean: 
	rm -f *.o core schedulerSim *.log

log : 
	cat Scheduler.log