#ifndef __PLIST_H
#define __PLIST_H

// ESTRUCTURA PNODE (PROCESS NODE) QUE REPRESENTA UN PROCESO.
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

// FUNCIONES DE PNODE
pnode* newNode(int pid, char* name);
char* getDateStr(struct tm* tm);
void printNode(pnode* node);

// ESTRUCTURA PLIST (PROCESS LIST), ES UNA LISTA DE PROCESOS.
typedef struct plist{
	pnode* first;
	pnode* last;
	int size;
}plist;

// FUNCIONES DE PLIST
plist* newList();
void deleteList(plist* list);
void size(plist* list);
pnode* find(int pid, plist* list);
void insert(int pid, char* name, plist* list);
void erase(int pid, plist* list);

void setAlarm(int pid, plist* list, int sec); // ESTABLECE UNA ALARMA.
void alarmDaemon(plist* list); // RECORRE LA LISTA EN BUSCA DE PROCESOS FINALIZADOS.
void killAll(plist* list); // MATA TODOS LOS PROCESOS DE LA LISTA.

void printList(plist* list);

#endif
