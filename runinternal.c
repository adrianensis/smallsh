#include "smallsh.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int isInternal(char* command){
	return(strcmp(command,"cd") == 0 
	|| strcmp(command,"exit") == 0
	|| strcmp(command,"bgproc") == 0
	|| strcmp(command,"alarma") == 0
	|| strcmp(command,"otherwc")== 0);
}


// PROPUESTA PARA MODULAR CÓDIGO, YA QUE PARA FUNCIONES ANTERIORES NO SE NOTABA
// PERO PARA HACER WC, HACE FALTA METER BASTANTE CODIGO:
// PODRIAMOS HACER QUE EL ISINTERNAL DEVUELVA UN VALOR ENTRE 1 Y EL TOTAL DE INSTRUCCIONES QUE TENEMOS
// DE MANERA QUE SE LLAME AL RUNINTERNAL CON ESE VALOR, Y ENTONCES TENGAMOS UN ARRAY EN EL RUNINTERAL
// DONDE EN CADA POSICION SE DEVUELVA UN STRING CON LA FUNCION A LA QUE HAY QUE LLAMAR O UN PUNTERO A FUNCION
// DE ESTA MANERA PODEMOS TENER UNA CLASE PARA CADA INSTRUCCION, CON LAS FUNCIONES AUXILIARES QUE NECESITE
// CADA FUNCION.

int runinternal(char **cline) {
	
     if((strcmp(cline[0],"cd") == 0)){
     	if(cline[1] != 0){
     		chdir(cline[1]);
     		getcwd (currentDirectory, 128);
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
    		
    }else if(strcmp(cline[0],"alarma") == 0){
    	if(cline[1] != NULL && cline[2] != NULL)
    		setAlarm(atoi(cline[1]), procList, atoi(cline[2]));
    }else if(strcmp(cline[0],"otherwc") == 0){		// si el segundo parametro no existe entonces se llama a otherwc sin opciones
    	if(cline[2] == NULL){
    		int fd = open (cline[1], O_RDONLY );
			if (fd == -1){
				perror ("read");
				exit(EXIT_FAILURE);
    		}
    			
    		struct stat st;
			stat(cline[1], &st);
			
    		int tam_buf = st.st_size;
    		char *buf[tam_buf];
    		int r;
    		
    		while((r=read(fd, buf, tam_buf)) > 0); // lee todo el fichero
   			
   			int wordCounter = 0;
   			int lineCounter = 0;
   			int i;
   			int characterBefore = 0;
   			
   			for(i=0; i < tam_buf; i++){
   				/*if(characterBefore && (strcmp(buf[i]," ")==0) )
   					wordCounter++;
   				else if (strcmp(buf[i],"\n") == 0)
   					lineCounter++;
   				else if (strcmp(buf[i]," ") == 1)
   					characterBefore = 1;*/
   			}
   						
   			printf("%d %d %d %s\n", tam_buf, wordCounter, lineCounter, cline[1]);		
    	}
    }
     		
     return 0;
}
