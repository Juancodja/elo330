CC=gcc
JAVAC=javac
CFLAG=-g -Werror -Wextra


all: recuento acumulador


acumulador: acumulador.o
	$(CC) $(CFLAG) acumulador.o -o acumulador -lrt 

acumulador.o: acumulador.c
	$(CC) $(CFLAG) -c acumulador.c


recuento: recuento.o
	$(CC) $(CFLAG) recuento.o -o recuento -lrt
 
recuento.o: recuento.c
	$(CC) $(CFLAG) -c recuento.c




clear:
		rm -rf *.o *.out *.class recuento acumulador