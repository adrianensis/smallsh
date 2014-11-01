#include <stdlib.h>
#include <string.h>

#define RESIZEPERCENT 0.20;

// Estructura que contendra el array, un par clave-valor, donde la clave es un string y el valor un puntero a funcion
// con una lista variable de parametros
typedef void (*pFunction)(char * , ...) value;
typedef struct Set{
	char *key;
	value pFunction; // Puntero a funcion con argumentos variables
} Set;

// Estructura que almacena el array de sets, guarda el tamano del array y el numero de elementos que contiene
typedef struct Map{
	int arraySize;
	int nElems;
	Set *array;
} Map;

// Funcion para crear un nuevo mapa con un array vacio de un tamano a elegir
// El mapa no es generico en cuanto a los tipos que contiene
Map* newMap(int arraySize){
	Map *map = malloc(sizeof(Set) * arraySize);
	map->arraySize = arraySize;
	map->array = (Set*) malloc(sizeof(Set)* arraySize);
	map->nElems = 0;
	return map;
}

// Funcion para incrementar el tamano del array en un porcentaje establecido por RESIZEPERCENT
void resizeMap(Map map){
	map.arraySize = map.arraySize * RESIZEPERCENT;
	map.array = (Set*) realloc(map.array, sizeof(Set) * map.arraySize);
}

// Funcion para desplazar los conjutos de un array
void move(Map map, int since, int to){
	int i;

	// si solo queda un hueco libre se aumenta el tamano del array un 20%
	if (to+1 == map.arraySize){
		resizeMap(map);
	}

	for(i= to+1 ; i> since; i--){
		map.array[i] = map.array[i-1];
	}
}

// Funcion auxiliar para insertar por orden en el arrayMap
void insertInOrder(Map map, Set set){
	int i = 0;
	int menor = 0;
	while(menor == 0){

		if(strcmp(map.array[i].key, set.key) < 0){
			i++;
		}else if (strcmp(map.array[i].key, set.key) >= 0){
			menor = 1;
			move(map, i, map.nElems);
			map.array[i].key = set.key;
			map.array[i].value = set.(&pFunction)();
		} // en otro caso, ya esta insertado y no se hace nada
	}
}


// Funcion para insertar nuevos elementos al mapa indicando el par clave-valor
void insertSet(Map map, char *key, value pFunction{
	Set set;
	set.key = key;
	set.pFunction = pFunction;

	map.nElems += 1;

	if(map.nElems == 0){	// si el mapa no tiene aun elementos se inserta la primera posicion de array
		map.array[0] = set;
	} else { // en caso de no estar vacio hay que buscar donde colocar en orden
		insertInOrder(map,set);
	}

}

void deleteSet(char *key){
	// Implementar, necesario?
}

// Funcion que realiza la busqueda binaria de una clave y devuelve su valor
// void (*(*searchSet)(Map map, char *key))(char *key)
void searchSet(Map map, char *key){ // como devuelvo puntero a funcion?
	int first = 0;
	int last = map.nElems-1;
	int middle = (first+last)/2;

	void (*funcPointer)(char *param , ... ) ;

	int found = 0;

   	while( (first <= last) && !found ){
      	if ( strcmp(map.array[middle].key,key) < 0)
        	first = middle + 1;
      	else if ( strcmp(map.array[middle].key,key) == 0 ) {
         	funcPointer = map.array[middle].pFunction;
         	found = 1;
      	}
      	else
         	last = middle - 1;

        if(!found)
      		middle = (first + last)/2;
   }

   if ( first > last )
      funcPointer = NULL;

  	return funcPointer;
}

