#include "smallsh.h"

void
user_inicializar(void)
{
    currentDirectory = malloc(128);
    initDirectory = malloc(128);
    getcwd (initDirectory, 128); // TODO 128 o otro?
    strcpy(currentDirectory, initDirectory); // El actual es el inicial.
    
    promptStr = NULL;
    
	time(&timeSeconds); // inicializamos tiempo.
	
	procList = newList(); // inicializamos la lista de procesos.
	
	dictionary = newTrieTree(); // inicializamos el diccionario de comandos.
	loadTrieTree(dictionary, "/usr/bin");
	loadTrieTree(dictionary, currentDirectory);
	
	cmdHistory = newStrList(); // inicializamos la lista historial de comandos.
}

void
user_finalizar(void)
{
    /* Liberar */
    alarm(0); // cancelamos la alarma del demonio.
    free(currentDirectory);
    free(initDirectory);
    deleteList(procList); // borramos lista de procesos.
    deleteTrieTree(dictionary);
    deleteStrList(cmdHistory);
    
    /*Despedida*/
    setColor(COLOR_BACK_BLUE);
    printf("\n**************\n*    BYE!    *\n**************\n");
    setColor(COLOR_RESET);
    printf("\n");
    
    //free(promptStr); //no hace falta, se libera en cada nueva linea.
}

char *
user_getPrompt(void)
{
    /* Implementación */
    time(&timeSeconds); // actualizar tiempo
    
    // el num de procesos es la longitud de la lista de procesos.
    char* numProcStr = malloc(5);
    sprintf (numProcStr, "%d ", procList->size);
    
    // obtenemos la fecha.
    struct tm* t = malloc(sizeof(struct tm));
	t = gmtime(&timeSeconds);
	char* dateStr = getDateStr(t);
    
    // Obtenemos el usuario. getLogin() devuelve el usuario real de UNIX, osea se tu.
    // No es ninguna funcion interna de smallsh.
    char* userStr;
    userStr = getlogin();
    
    // RELLENAMOS EL PROMPT
    promptStr = malloc(300);
    
    int i, j;
    j = 0;
    for(i=0; i<strlen(numProcStr); i++){
    	promptStr[j] = numProcStr[i];
    	j++;
    }
   
    for(i=0; i<strlen(dateStr)-1; i++){
    	promptStr[j] = dateStr[i];
    	j++;
    }
    
    promptStr[j]=' '; 
    j++;
    	
    for(i=0; i<strlen(userStr); i++){
    	promptStr[j] = userStr[i];
    	j++;
    }
    
    promptStr[j]='@'; 
    j++;
    
    for(i=0; i<strlen(currentDirectory); i++){
    	promptStr[j] = currentDirectory[i];
    	j++;
    }
    
    // liberamos las cadenas que ya no nos valen.
    free(numProcStr);
    free(dateStr);
    
    return promptStr;
}

char *
user_flecha(int direccion, char* patron)
{
	char* cmd;
	
	if(direccion == FLECHA_ARRIBA)
    	cmd = prevStr(cmdHistory);
    else if(direccion == FLECHA_ABAJO)
    	cmd = nextStr(cmdHistory);
    	
    if(cmd == NULL){
    	cmd = malloc(1);
    	cmd[0] = '\0';
    	return cmd;
    }else
    	return cmd;
}

void
user_nueva_orden(char * orden)
{
	resetCurrent(cmdHistory);
	if(strlen(orden) > 0)
    	pushBack(cmdHistory, orden);
}

char *
user_tabulador(char * parte, int numero, int numtab)
{
    /* Implementación */
    // TODO ?
    
   	if((strlen(parte) > 0) && (numtab == 2))
   		autoComplete(dictionary, parte);

    return NULL;
}

/* $Id: userfn.c 1399 2007-12-20 09:45:07Z pedroe $ */
