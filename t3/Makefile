CC=gcc
JAVAC=javac
CFLAG=-g -Werror -Wextra


all: recuento acumulador visualizador


acumulador: acumulador.o
	$(CC) $(CFLAG) acumulador.o -o acumulador -lrt -lpthread

acumulador.o: acumulador.c
	$(CC) $(CFLAG) -c acumulador.c


recuento: recuento.o
	$(CC) $(CFLAG) recuento.o -o recuento -lrt
 
recuento.o: recuento.c
	$(CC) $(CFLAG) -c recuento.c

visualizador: visualizador.java
	$(JAVAC) visualizador.java


clear:
		rm -rf *.o *.out *.class recuento acumulador