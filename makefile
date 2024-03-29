CC = gcc
CFLAGS = -g -Wall -pedantic

main : main.o printStats.o 
	${CC} ${CFLAGS} -o schedulerSim main.o printStats.o 
main.o : printStats.o a2.h

printStats.o : a2.h

#myPrintStats.o : a2.h

test: main
	./schedulerSim a2in.txt


.PHONY : clean log remake


clean: 
	rm -f *.o core schedulerSim *.log

log : 
	cat scheduler.log

remake :
	make clean
	make