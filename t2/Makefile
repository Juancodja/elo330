CC=gcc
CFLAG=-g -Werror -Wextra

all: cfifo pfifo cshmem pshmem

cfifo: cfifo.o
	$(CC) $(CFLAG) cfifo.o -o cfifo -lrt
 
cfifo.o: cfifo.c
	$(CC) $(CFLAG) -c cfifo.c


pfifo: pfifo.o
	$(CC) $(CFLAG) pfifo.o -o pfifo -lrt 

cpifo.o: pfifo.c
	$(CC) $(CFLAG) -c pfifo.c


cshmem: cshmem.o
	$(CC) $(CFLAG) cshmem.o -o cshmem -lrt -pthread

cshmem.o: cshmem.c
	$(CC) $(CFLAG) -c cshmem.c


pshmem: pshmem.o
	$(CC) $(CFLAG) pshmem.o -o pshmem -lrt -pthread

pshmem.o: pshmem.c
	$(CC) $(CFLAG) -c pshmem.c

clear:
		rm -rf *.o *.out *.dat cfifo pfifo fifo cshmem pshmem