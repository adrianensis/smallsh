#include "strlist.h"

strnode* newStrNode(char* str){
	strnode* node = (strnode*)malloc(sizeof(strnode));
	node->next = NULL;
	node->prev = NULL;
	node->str = strdup(str);
}

void deleteStrNode(strnode* node){
	printf("Destruyendo: %s\n", node->str);
	getchar();
	free(node->str);
	free(node);
}

strlist* newStrList(){
	strlist* list = (strlist*)malloc(sizeof(strlist));
	list->first = NULL;
	list->last = NULL;
	list->selected = NULL;
	list->size = 0;
}

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

void pushBack(strlist* list, char* str){

	strnode* node = newStrNode(str);

	if(list->first == NULL){
		list->first = node;
		list->selected = list->first;
	}else{
		list->last->next = node;
	}
	
	if(list->last != NULL){
		list->last->next = node;
		node->prev = list->last;
		node->next = NULL;
	}
	
	list->last = node;
	list->size++;
		
}

char* getCurrent(strlist* list){
	return list->selected->str;
}

void nextStr(strlist* list){
	if((list->size > 0) && (list->selected->next != NULL))
		list->selected = list->selected->next;
}

void prevStr(strlist* list){
	if((list->size > 0) && (list->selected->prev != NULL))
		list->selected = list->selected->prev;
}
