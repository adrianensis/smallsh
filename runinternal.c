#include "smallsh.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <ctype.h>


// Variables contadoras parciales
int charCounter = 0;
int wordCounter = 0;
int lineCounter = 0;

// variables contadoras totales
int totalCharCounter = 0;
int totalWordCounter = 0;
int totalLineCounter = 0;

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

// ENTIENDO LO QUE DICES PERO CREO QUE LO DE LOS PUNTEROS A FUNCIONES SE IBA A COMPLICAR MUCHO,
// YO AL MENOS NO SE MANEJAR AUN BIEN LOS PUNTEROS A FUNCIONES, EN C SON SUPER RAROS.
// LO QUE SI PODEMOS HACER DE MOMENTO ES UNA FUNCION PARA WC, LA DECLARAMOS FUERA DE RUNINTERNAL
// Y ESO YA ACLARA BASTANTE EL CODIGO. PARA EXIT Y BGPROC TAMBIEN SE HA HECHO (las funciones que se usan estan
// declaradas en los ficheros plist.c y plist.h)
// ↓↓↓↓↓PODEMOS DECLARARLA AQUI MISMO↓↓↓↓↓

// SE LE PUEDE CAMBIAR EL NOMBRE, ES POR PONER ALGO.
void otherwcFunc(){

}

void countChar(char c){
  charCounter++;
  if(c == '\n')
    lineCounter++;
}

int getWord(char *buf[], int offset){
  int i = offset;
  char c;
  int stop = 0;

printf("Llego antes del primer bucle de getword\n");
  // Si el caracter es una letra, se contabiliza como palabra
  // y se sale de este bucle para contar todas las letras que tiene la palabra
  // si no es un caracter va contnado todos los espacios.
  while(((c = buf[i]) != EOF) && !stop){
    if(isalpha(c)){
      wordCounter++;
      stop = 1;
    }
    if(!stop){
      charCounter++;
      i++;
    }
  }

  if(c == EOF)
    return -1;

  stop = 0;

printf("Llego antes del segundo bucle\n");
  // cuenta todas las letras de la palabra
  // si es un espacio lo cuenta y se sale del bucle.
  while(((c = buf[i]) != EOF) && !stop){
    countChar(c);
    i++;
    if(!isalpha(c)){
      stop = 1;
    }

  }

  // si es EOF lo indica y si no devuelve por donde debe continuar la siguiente palabra
  return (c != EOF)? offset+1 : -1;

}

void counterFile(char *file){
  // abre el fichero
  int fd = open (file, O_RDONLY );
  // se obtiene su estado
  struct stat st;

  stat(file, &st);

  int tam_buf = st.st_size;
  char *buf[tam_buf];
  int r;

  while((r=read(fd, buf, tam_buf)) > 0); // lee todo el fichero

  charCounter = 0;
  lineCounter = 0;
  wordCounter = 0;

  int offset = 0;

printf("Llego antes de empezar a leer todas las palabras\n");
  // se van leyendo todas las palabras
  while((offset = getWord(buf, offset)) != -1);

  printf ("%d %d %d %s\n", charCounter, wordCounter, lineCounter, file);

  // cierra fichero
  if (close(fd) == -1){
    perror("read");
    exit(EXIT_FAILURE);
  }
}

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

      // se abre el fichero
      if(cline[2] == NULL){
    		int fd = open (cline[1], O_RDONLY );
			if (fd == -1){
				perror ("read");
				exit(EXIT_FAILURE);
    	}

        printf("Llego antes de counterFile\n");
        counterFile(cline[1]);

    	}
    }

     return 0;
}
