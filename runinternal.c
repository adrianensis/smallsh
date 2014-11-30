#include "smallsh.h"

#define INITIAL_SIZE 7

// Estructura de datos que contiene todas las ordenes internas

int isInternal(char* command){
	return(strcmp(command,"cd") == 0
	|| strcmp(command,"exit") == 0
	|| strcmp(command,"bgproc") == 0
	|| strcmp(command,"alarma") == 0
	|| strcmp(command,"otherwc")== 0
	|| strcmp(command,"findbysize")== 0);
}

int runinternal(char **cline) {

     if((strcmp(cline[0],"cd") == 0)){
     	if(cline[1] != 0){
     		chdir(cline[1]);
     		getcwd (currentDirectory, 128);

     		// Recargamos diccionario.
			deleteTrieTree(dictionary);
			dictionary = newTrieTree();
			loadTrieTree(dictionary, "/usr/bin");
			loadTrieTree(dictionary, currentDirectory);
			addStr(dictionary, "cd");
			addStr(dictionary, "exit");
			addStr(dictionary, "bgproc");
			addStr(dictionary, "alarma");
			addStr(dictionary, "otherwc");
			addStr(dictionary, "findbysize");

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
        otherwc(cline);
    }else if((strcmp(cline[0],"findbysize") == 0) && (cline[1] != NULL) && (cline[2] != NULL)){
		    findbysize(cline);
    }

     return 0;
}
