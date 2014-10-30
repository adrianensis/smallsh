#include "plist.h"

pnode* newNode(int pid, char* name){
	pnode* node =(pnode*) malloc(sizeof(pnode));
	node->next = NULL;
	node->prev = NULL;
	node->pid = pid;
	node->name = malloc(strlen(name));
	strcpy(node->name, name);
	
	time_t t;
	time(&t);
	node->initTime = t;
	node->initDate = malloc(sizeof(struct tm));
	node->initDate = gmtime(&t);
	node->totalTime = 0;
	node->untilAlarm = -1;

	return node;
}

plist* newList(){
	plist* list =(plist*) malloc(sizeof(plist));
	list->first = NULL;
	list->last = NULL;
	list->size = 0;
	
	return list;
}

void deleteList(plist* list){
	if(list->first != NULL){
		pnode* it = list->first;
		pnode* aux = it;
		while(it->next != NULL){
			it = it->next; // avanzar
			free(aux);
			aux = it;
		}
	
		free(it);
		free(list);
	}
}

pnode* find(int pid, plist* list){
	if(list->first != NULL){
		pnode* it = list->first; 
		while((it->next != NULL) && (it->pid != pid))
			it = it->next; // avanzar
	
		if(it->pid == pid)
			return it;
		else
			return NULL;
	}else
		return NULL;
}

void insert(int pid, char* name, plist* list){
	pnode* node = newNode(pid, name);
	
	/*If empty*/
	if(list->first == NULL){
		list->first = node;
		list->last = node;
		list->size++;
		
	}else{
		
		/*Find*/
		pnode* it = list->first; 
		while((it->next != NULL) && (it->pid <= pid)){
			it = it->next; // avanzar 
		}	
		
		// TODO controlar si ya existe?
		/*Insert*/
		if(it->pid > pid){
			if(it->prev == NULL){ // si es el primero
				it->prev = node;
				node->next = it;
				list->first = node;
			}else{
				pnode* it_prev = it->prev;
			
				it->prev = node;
				node->next = it;
			
				it_prev->next = node;
				node->prev = it_prev;
			}
			
		}else{
			it->next = node;
			node->prev = it;
			list->last = node;
		}
		
		list->size++;
	}
	
}

void erase(int pid, plist* list){

	pnode* it;
	it = find(pid, list);
	if(it != NULL){
		if((it->prev != NULL) && (it->next != NULL)){
			it->prev->next = it->next;
			it->next->prev = it->prev;
			
		}else if(it->prev != NULL){
			it->prev->next = NULL;
			list->last = it->prev;
			
		}else if(it->next != NULL){
			it->next->prev = NULL;
			list->first = it->next;
		}else{
			list->first = NULL;
			list->last = NULL;
		}
		
		free(it);
		
		list->size--;
	}
	 	
	 
	 
}

void setAlarm(int pid, plist* list, int sec){
	pnode* it = find(pid, list);
	if(it != NULL)
		it->untilAlarm = sec;
}


void alarmDaemon(plist* list){

	if(list != NULL){
	
		/*Buffer para almacenar los procesos terminados*/
		int i = 0;
		int n = 64; // capacidad del buffer
		int* buffer = malloc(sizeof(int)*n);

		time_t t;
	
		/*Bucle para decrementar*/
		pnode* it = list->first; 
		while(it != NULL){
	
			time(&t);
			it->totalTime = (t - it->initTime);
		
			if(it->untilAlarm > 0)
				it->untilAlarm = it->untilAlarm - 1;
			
			if(it->untilAlarm == 0){
				kill(it->pid,SIGINT);
				buffer[i] = it->pid;
				i++;
				
				printf("\n");
				setColor(COLOR_BACK_RED);
				printf("¡ALARMA TERMINADA!\n");
				printf("Proceso en segundo plano terminado forzosamente con la señal %d : [%d]", SIGINT, it->pid);
				setColor(COLOR_RESET);
				printf("\n");
			}
		
			it = it->next; // avanzar 
		}
	
		/*Bucle para limpiar lista*/
		int j;
		for(j=0; j<i; j++)
			erase(buffer[j], list);
		
	}
	
	//Restablecemos alarma para el demonio
	alarm(1);
}

void killAll(plist* list){
	if(list->first != NULL){
		setColor(COLOR_BACK_RED);
		pnode* it = list->first;
		pnode* aux = it;
		while(it->next != NULL){
			it = it->next; // avanzar
			kill(aux->pid,SIGINT);
			printf("Proceso en segundo plano terminado forzosamente con la señal %d : [%d] \n", SIGINT, aux->pid);
			//free(aux->time);
			free(aux);
			aux = it;
		}
		printf("Proceso en segundo plano terminado forzosamente con la señal %d : [%d] \n", SIGINT, aux->pid);
		kill(it->pid,SIGINT);
		free(it);
		list->first = NULL;
		list->last = NULL;
		setColor(COLOR_RESET);
	}
}

char* getDateStr(struct tm* tm){
	char* tmp = malloc(17);
	sprintf(tmp,"%d/%d/%d %d:%d:%d",tm->tm_mday,tm->tm_mon,tm->tm_year-100,(tm->tm_hour + 1) % 24,tm->tm_min,tm->tm_sec);
	return tmp;
}

void printNode(pnode* node){
	printf("%s\t", node->name);
	char* date = getDateStr(node->initDate);
	printf("%s\t", date);
	printf("%d\t",node->totalTime);
	printf("%d\t",node->untilAlarm);
	printf("\n");
	free(date);
}

void printList(plist* list){
	
	if(list->first != NULL){
		pnode* it;
		for(it = list->first; it->next != NULL; it = it->next ){
			printNode(it);
		}
		printNode(it);
	}
}
