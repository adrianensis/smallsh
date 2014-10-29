#ifndef __PLIST_H
#define __PLIST_H

#include <stdlib.h>
#include <stdio.h>
#include <sys/signal.h>
#include <string.h>
#include <time.h>
#include "color.h"

typedef struct pnode{
	/*Punteros a nodos hermanos*/
	struct pnode* next;
	struct pnode* prev;
	
	/*Atributos*/
	int pid;
	char* name;
	time_t initTime;
	struct tm* initDate;
	int totalTime;
	int untilAlarm;  
}pnode;

pnode* newNode(int pid, char* name);
char* getDateStr(struct tm* tm);
void printNode(pnode* node);

typedef struct plist{
	pnode* first;
	pnode* last;
	int size;
}plist;

plist* newList();
void deleteList(plist* list);
void size(plist* list);
pnode* find(int pid, plist* list);
void insert(int pid, char* name, plist* list);
void erase(int pid, plist* list);

void setAlarm(int pid, plist* list, int sec);
void alarmDaemon(plist* list);
void killAll(plist* list);

void printList(plist* list);

#endif
