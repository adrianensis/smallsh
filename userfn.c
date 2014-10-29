#include "smallsh.h"

void
user_inicializar(void)
{
    currentDirectory = malloc(128);
    initDirectory = malloc(128);
    getcwd (initDirectory, 128); // TODO 128 o otro?
    strcpy(currentDirectory, initDirectory); // El actual es el inicial.
    
    promptStr = NULL;
    
	time(&timeSeconds);
	
	procList = newList();
}

void
user_finalizar(void)
{
    /* Liberar */
    alarm(0); // cancelamos la alarma del demonio.
    free(currentDirectory);
    free(initDirectory);
    deleteList(procList);
    
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
    
    char* numProcStr = malloc(5);
    sprintf (numProcStr, "%d ", procList->size);
    
    struct tm* t = malloc(sizeof(struct tm));
	t = gmtime(&timeSeconds);
	char* dateStr = getDateStr(t);
    
    char* userStr;
    userStr = getlogin();
    
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
    
    free(numProcStr);
    free(dateStr);
    
    return promptStr;
}

char *
user_flecha(int direccion, char* patron)
{
    /* Implementación */
    return NULL;
}

void
user_nueva_orden(char * orden)
{
    /* Implementación */
}

char *
user_tabulador(char * parte, int numero, int numtab)
{
    /* Implementación */
    return NULL;
}

/* $Id: userfn.c 1399 2007-12-20 09:45:07Z pedroe $ */
