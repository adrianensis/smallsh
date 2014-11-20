prueba: main.c strlist.o 
	gcc main.c strlist.o -o prueba

strlist.o: strlist.c
	gcc -c strlist.c

clean:
	rm *.o prueba
	
	#! /usr/bin/make -f
# Fichero Makefile para construir el ejecutable smallsh

CFLAGS=-g -Wall

OBJECTS=fileutil.o findbysize.o trietree.o color.o 

smallsh: $(OBJECTS)

# nuevo
color.o: color.c
fileutil.o: fileutil.c
trietree.o: trietree.c

clean:
	rm -rf *~ $(OBJECTS) 
# $Id: Makefile 1398 2007-12-20 08:06:01Z pedroe $
