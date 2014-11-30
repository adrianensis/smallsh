#include "strlist.h"

// Constructor de nodo.
strnode* newStrNode(char* str){
	strnode* node = (strnode*)malloc(sizeof(strnode));
	node->next = NULL;
	node->prev = NULL;
	node->str = strdup(str);
	
	return node;
}

// Destructor de nodo.
void deleteStrNode(strnode* node){
	free(node->str);
	free(node);
}

// Constructor de lista.
strlist* newStrList(){
	strlist* list = (strlist*)malloc(sizeof(strlist));
	list->first = NULL;
	list->last = NULL;
	list->selected = NULL;
	list->size = 0;
	
	return list;
}

// Destructor de lista.
void deleteStrList(strlist* list){

	strnode* it, *aux;
	if(list->size > 0){
		it = list->last;
		while(it->prev != NULL){
			aux = it;
			it = it->prev;
			deleteStrNode(aux);
		}
		deleteStrNode(it);
	}

	free(list);
}

// Inserta una cadena al final de la lista.
void pushBack(strlist* list, char* str){

	strnode* node = newStrNode(str);

	if(list->first == NULL){
		list->first = node;
	}else{
		list->last->next = node;
		node->prev = list->last;
	}
	
	list->selected = node;
	list->last = node;
	list->size++;
		
}

// Devuelve la cadena seleccionada actualmente en la lista.
char* getCurrent(strlist* list){
	return list->selected->str;
}

// Avanza a la siguiente cadena y la establece como la actual seleccionada.
char* nextStr(strlist* list){
	if((list->size > 0) && (list->selected->next != NULL)){
		list->selected = list->selected->next;
		
		return strdup(list->selected->str);
	}else 
		return NULL;
}

// Retrocede a la anterior cadena y la establece como la actual seleccionada.
char* prevStr(strlist* list){
	if((list->size > 0) && (list->selected != NULL)){
		char* aux = strdup(list->selected->str);
		if(list->selected->prev != NULL)
			list->selected = list->selected->prev;
		return aux;
	}else
		return NULL;
}

// Devuelve el puntero de cadena seleccionada a la cabeza de la lista.
void resetCurrent(strlist* list){
	list->selected = list->first;
}

// Imprime todos los comandos de la lista indexados numéricamente.
void printHistory(strlist* list){
	
	int counter = 0;
	strnode* it = list->first;
	
	while(it->next != NULL){
		printf("%d %s\n", counter+1, it->str);
		it = it->next;
		counter++;
	}
		
}
