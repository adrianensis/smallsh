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


// PROPUESTA PARA MODULAR CÓDIGO, YA QUE PARA FUNCIONES ANTERIORES NO SE NOTABA
// PERO PARA HACER WC, HACE FALTA METER BASTANTE CODIGO:
// PODRIAMOS HACER QUE EL ISINTERNAL DEVUELVA UN VALOR ENTRE 1 Y EL TOTAL DE INSTRUCCIONES QUE TENEMOS
// DE MANERA QUE SE LLAME AL RUNINTERNAL CON ESE VALOR, Y ENTONCES TENGAMOS UN ARRAY EN EL RUNINTERAL
// DONDE EN CADA POSICION SE DEVUELVA UN STRING CON LA FUNCION A LA QUE HAY QUE LLAMAR O UN PUNTERO A FUNCION
// DE ESTA MANERA PODEMOS TENER UNA CLASE PARA CADA INSTRUCCION, CON LAS FUNCIONES AUXILIARES QUE NECESITE
// CADA FUNCION.

// ENTIENDO LO QUE DICES PERO CREO QUE LO DE LOS PUNTEROS A FUNCIONES SE IBA A COMPLICAR MUCHO,
// YO AL MENOS NO SE MANEJAR AUN BIEN LOS PUNTEROS A FUNCIONES, EN C SON SUPER RAROS.
// LO QUE SI PODEMOS HACER DE MOMENTO ES UNA FUNCION PARA WC, LA DECLARAMOS FUERA DE RUNINTERNAL
// Y ESO YA ACLARA BASTANTE EL CODIGO. PARA EXIT Y BGPROC TAMBIEN SE HA HECHO (las funciones que se usan estan
// declaradas en los ficheros plist.c y plist.h)
// ↓↓↓↓↓PODEMOS DECLARARLA AQUI MISMO↓↓↓↓↓


//TODO discutir si es mejor usar los flags locales e ir pasandolos como parametros o bien
// hacerlos variables globales.


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
