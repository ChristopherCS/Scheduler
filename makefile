CC = gcc
CFLAGS = -g -Wall

OperatingSystemsA1 : OperatingSystemsA1.o phones.o persons.o linkedList.o

OperatingSystemsA1.o : OperatingSystemsA1.h phones.o persons.o linkedList.o

phones.o persons.o : phones.h persons.h linkedList.o


linkedList.o : linkedList.h



clean: 
	rm -f *.o core OperatingSystemsA1