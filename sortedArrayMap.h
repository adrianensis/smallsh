#ifndef __SORTEDARRAYMAP_H
#define __SORTEDARRAYMAP_H

#include <stdlib.h>
#include <string.h>

#define RESIZEPERCENT 0.20

// Estructura que contendra el array, un par clave-valor, donde la clave es un string y el valor un puntero a funcion
// con una lista variable de parametros
typedef void (*pFunction)(char * , ...);

typedef struct Set{
	char *key;
	pFunction value; // Puntero a funcion con argumentos variables
} Set;

// Estructura que almacena el array de sets, guarda el tamano del array y el numero de elementos que contiene
typedef struct Map{
	int arraySize;
	int nElems;
	Set *array;
} Map;


void newMap(Map*, int );
void resizeMap(Map map);
void move(Map map, int , int );
void insertInOrder(Map , Set);
void insertSet(Map , char *, pFunction);
void deleteSet(char *);
pFunction searchSet(Map , char *);

#endif