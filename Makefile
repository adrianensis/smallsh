#! /usr/bin/make -f
# Fichero Makefile para construir el ejecutable smallsh

CFLAGS=-g -Wall
LDLIBS=-lfl # Para flex

OBJECTS=smallsh.o runinternal.o fileutil.o findbysize.o trietree.o strlist.o color.o plist.o gettok.o procline.o runcom.o buffer.o userfn.o shell.o

smallsh: $(OBJECTS)

# nuevo
runinternal.o: runinternal.c 
color.o: color.c
fileutil.o: fileutil.c
findbysize.o: findbysize.c
trietree.o: trietree.c
strlist.o: strlist.c


plist.o: plist.c

gettok.o: gettok.c

procline.o: procline.c

runcom.o: runcom.c

smallsh.o: smallsh.c

buffer.o: buffer.c

userfn.o: userfn.c

shell.c: shell.l

clean:
	rm -rf *~ $(OBJECTS) shell.c smallsh core

# $Id: Makefile 1398 2007-12-20 08:06:01Z pedroe $
