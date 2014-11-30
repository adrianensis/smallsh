#include "smallsh.h"

void
user_inicializar(void)
{
    currentDirectory = malloc(128);
    initDirectory = malloc(128);
    getcwd (initDirectory, 128);
    strcpy(currentDirectory, initDirectory); // El actual es el inicial.

    promptStr = NULL;

	time(&timeSeconds); // inicializamos tiempo.

	procList = newList(); // inicializamos la lista de procesos.

	// inicializamos el diccionario de palabras.
	dictionary = resetTrieTree(currentDirectory);
	
	if(dictionary == NULL)
		printf("nulo\n");
	
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

    time_t timer;
    char dateStr[25];
    struct tm* tm_info;

    time(&timer);
    tm_info = localtime(&timer);

    strftime(dateStr, 25, "%d/%m/%Y %H:%M:%S", tm_info);

    // Obtenemos el usuario. getLogin() devuelve el usuario real de UNIX, osea se tu.
    // No es ninguna funcion interna de smallsh.
    char* userStr;
    userStr = getlogin();

    // RELLENAMOS EL PROMPT
    promptStr = malloc(300);

    int i, j;
    j = 0;

    // Se agrega al inicio del prompt el color

    for(i=0; i<strlen(COLOR_BOLD_GREEN); i++){
        promptStr[j] = COLOR_BOLD_GREEN[i];
        j++;
    }

    for(i=0; i<strlen(numProcStr); i++){
        promptStr[j] = numProcStr[i];
        j++;
    }

    for(i=0; i<strlen(dateStr); i++){
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

    promptStr[j]='$';
    j++;

    // Se resetea el color para los comandos sucesivos

    for(i=0; i<strlen(COLOR_RESET); i++){
        promptStr[j] = COLOR_RESET[i];
        j++;
    }

    // liberamos las cadenas que ya no nos valen.
    free(numProcStr);

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
	char* word = NULL;
   	if((strlen(parte) > 0) && (numtab == 1)){
   		free(word);
   		word = NULL;
   		word = autoComplete(dictionary, parte);	
   	}else if((strlen(parte) > 0) && (numtab == 2))
   		predict(dictionary, parte);

    return word;
}

/* $Id: userfn.c 1399 2007-12-20 09:45:07Z pedroe $ */
