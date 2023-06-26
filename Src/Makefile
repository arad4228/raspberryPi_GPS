CC=g++
CFLAGS=-o
LDLIBS=-lwiringPi
OUTPUTDIR=../Build

all:gps

CGPS.o: CGPS.h CGPS.cpp

main.o: CGPS.h main.cpp

gps: CGPS.o main.o
	$(CC) $^ $(LDLIBS) -o ${OUTPUTDIR}/$@

clean:
	rm -f *.o