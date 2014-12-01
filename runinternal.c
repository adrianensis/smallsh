#include "smallsh.h"

#define INITIAL_SIZE 7
#define MAX_LONG 120

// Estructura de datos que contiene todas las ordenes internas

int isInternal(char* command){
	return(strcmp(command,"cd") == 0
	|| strcmp(command,"exit") == 0
	|| strcmp(command,"bgproc") == 0
	|| strcmp(command,"alarma") == 0
	|| strcmp(command,"otherwc")== 0
	|| strcmp(command,"findbysize")== 0
	|| strcmp(command,"history")== 0);
}

int runinternal(char **cline) {

	int error = 0;

     if((strcmp(cline[0],"cd") == 0)){
     	if(cline[1] != 0){
     		chdir(cline[1]);
     		getcwd (currentDirectory, 128);

     		// Recargamos diccionario.
     		deleteTrieTree(dictionary);
			dictionary = resetTrieTree(currentDirectory);

     	}else{
     		chdir(initDirectory);
     		getcwd (currentDirectory, 128);
     	}
    }else if(strcmp(cline[0],"exit") == 0){
		/* Antes de salir debemos esperar a que los hijos
			en segundo plano acaben.
		*/
		killAll(procList);

    }else if(strcmp(cline[0],"bgproc") == 0){
    	printf("-------------- BACKGROUND PROCESSES --------------\n");
		printList(procList);
		printf("--------------------------------------------------\n");

    }else if((strcmp(cline[0],"alarma") == 0) && (cline[1] != NULL) && (cline[2] != NULL)){
    		setAlarm(atoi(cline[1]), procList, atoi(cline[2]));
    }else if(strcmp(cline[0],"otherwc") == 0){		// si el segundo parametro no existe entonces se llama a otherwc sin opciones
            error = otherwc(cline);
    }else if(strcmp(cline[0],"findbysize") == 0){
		    error = findbysize(cline);
    }else if(strcmp(cline[0],"history") == 0){
    	if(cline[1] == NULL)
    		printHistory(cmdHistory);
    }

     return error;
}
