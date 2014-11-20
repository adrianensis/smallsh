#ifndef __STRLIST_H
#define __STRLIST_H

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

// ESTRUCTURA strNODE (STRING NODE)
typedef struct strnode{
	/*Punteros a nodos hermanos*/
	struct strnode* next;
	struct strnode* prev;
	/*Atributos*/
	char* str;
}strnode;

// FUNCIONES DE STRNODE
strnode* newStrNode(char* str);
void deleteStrNode(strnode* node);

// ESTRUCTURA STRLIST (STRING LIST)
typedef struct strlist{
	strnode* first;
	strnode* last;
	strnode* selected; // seleccionado actualmente.
	int size;
}strlist;

// FUNCIONES DE STRLIST
strlist* newStrList();
void deleteStrList(strlist* list);
void pushBack(strlist* list, char* str);
char* getCurrent(strlist* list);
void nextStr(strlist* list);
void prevStr(strlist* list);

#endif