#include "strlist.h"

strnode* newStrNode(char* str){
	strnode* node = (strnode*)malloc(sizeof(strnode));
	node->next = NULL;
	node->prev = NULL;
	node->str = strdup(str);
	
	return node;
}

void deleteStrNode(strnode* node){
	free(node->str);
	free(node);
}

strlist* newStrList(){
	strlist* list = (strlist*)malloc(sizeof(strlist));
	list->first = NULL;
	list->last = NULL;
	list->selected = NULL;
	list->size = 0;
	
	return list;
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
	}else{
		list->last->next = node;
		node->prev = list->last;
	}
	
	list->selected = node;
	list->last = node;
	list->size++;
		
}

char* getCurrent(strlist* list){
	return list->selected->str;
}

char* nextStr(strlist* list){
	if((list->size > 0) && (list->selected->next != NULL)){
		list->selected = list->selected->next;
		
		return strdup(list->selected->str);
	}else 
		return NULL;
}

char* prevStr(strlist* list){
	if((list->size > 0) && (list->selected != NULL)){
		char* aux = strdup(list->selected->str);
		if(list->selected->prev != NULL)
			list->selected = list->selected->prev;
		return aux;
	}else
		return NULL;
}

void resetCurrent(strlist* list){
	list->selected = list->first;
}
