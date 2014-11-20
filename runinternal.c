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
int totalCharCounter;
int totalWordCounter;
int totalLineCounter;

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

// Funcion para mostrar la salida de otherwc
void showOutputWc(char *file, int lflag, int wflag, int cflag){

  int line, word, character;

  if (strcmp(file, "total") == 0){
    line = totalLineCounter;
    word = totalWordCounter;
    character = totalCharCounter;
  }else{
    line = lineCounter;
    word = wordCounter;
    character = charCounter;
  }

  // Para cuando no se dice nada, se consideran todas los flags activos
  if ((lflag == 0) && (wflag == 0) && (cflag == 0))
    lflag = wflag = cflag = 1;

  if (lflag)
    printf("%d ", line);
  if (wflag)
    printf("%d ", word);
  if (cflag)
    printf("%d ", character);
  printf("%s\n", file);

}

// TODO cada vez que se ejecuta un exit(1) o exit(EXIT_FAILURE) se producen errores en la terminal del sitema
// fuera de smallsh
void countChar(char c){
  charCounter++;
  if((c) == '\n'){
    lineCounter++;
  }
}

int getWord(char buf[], int offset, int sizeBuffer){
  int i = offset;
  char c;
  int stop = 0;

  // Si el caracter es una letra, se contabiliza como palabra
  // y se sale de este bucle para contar todas las letras que tiene la palabra
  // si no es un caracter va contnado todos los espacios.
  while(!stop && (i<sizeBuffer)){
    c = buf[i];
    if(isalpha(c)){
      wordCounter++;
      stop = 1;
    }

    if(!stop){
      countChar(c);
      i++;
    }

  }

  if(i == sizeBuffer)
    return -1;

  stop = 0;

  // cuenta todas las letras de la palabra
  // si es un espacio lo cuenta y se sale del bucle.
  while( !stop && (i < sizeBuffer)){
    c = buf[i];
    countChar(c);
    i++;
    if(!(isalpha(c))){
      stop = 1;
    }

  }

  // si es fin de sizeBuffer lo indica y si no devuelve por donde debe continuar la siguiente palabra
  return (i != sizeBuffer-1) ? i : -1;
}

void counterFile(char *file, int lflag, int wflag, int cflag){
  // abre el fichero
  int fd = open (file, O_RDONLY );
  // se obtiene su estado
  struct stat st;

  stat(file, &st);

  int tam_buf = st.st_size;
  char buf[tam_buf];
  int r;

  while((r=read(fd, buf, tam_buf)) > 0); // lee todo el fichero

  charCounter = 0;
  lineCounter = 0;
  wordCounter = 0;

  int offset = 0;

  // se van leyendo todas las palabras
  while((offset = getWord(buf, offset, tam_buf)) != -1);

  totalLineCounter += lineCounter;
  totalWordCounter += wordCounter;
  totalCharCounter += charCounter;

  showOutputWc(file, lflag, wflag, cflag);

}
// SE LE PUEDE CAMBIAR EL NOMBRE, ES POR PONER ALGO.
void otherwcFunc(char **cline){

  // Se inician los contadores totales a 0
  // Solo se muestran si hay mas de un fichero como parametro
  totalCharCounter = totalLineCounter = totalWordCounter = 0;

  int totalFlag = -1;
  int i = 1;
  int j = 1;

  //parametros inicialmente a 0
  int lflag, wflag, cflag;
  lflag = wflag = cflag = 0;

  // Se reconocen todos los flags de opciones
  while(cline[i][0] == '-'){
    switch(cline[i][j]){
      case 'l': lflag = 1; break;
      case 'w': wflag = 1; break;
      case 'c': cflag = 1; break;
      default : printf ("Uso: wc [-cwl] FILE [FILE ... ]  --> %c\n", cline[i][j]); break;
    }

    // avanza al siguiente caracter de una opcion para las del tipo (-lw)
    j++;

    // si no existen mas caracteres en esa opcion, se avanza al siguiente parametro
    if(cline[i][j] == '\0'){
      i++;
      j = 1;
    }
  }

  // se empiezan a tratar los ficheros
  while( cline[i] != NULL){

      // se abre el fichero
      int fd = open (cline[i], O_RDONLY );

      if (fd == -1){
        char outputError[100];
        sprintf(outputError, "%s: %s: No existe el fichero\n", cline[0], cline[i]);
        perror (outputError);
        exit(EXIT_FAILURE);
      }

      counterFile(cline[i], lflag, wflag, cflag);
      totalFlag++;
      i++;

      // cierra fichero
      if (close(fd) == -1){
        perror("read");
        exit(EXIT_FAILURE);
      }

  }

  // Si no se le pasa fichero como parametro se muestra la forma de hacerlo
  if(i < 2){

    printf ("Uso: wc [-cwl] FILE [FILE ... ]\n");
   // exit(EXIT_FAILURE); TODO - Cuando sale con fallo, no debe salir de la aplicacion,
  // ademas cuando sale toca algo que hace que la terminal normal del sistema (fuera de smallsh)
  // no funcione bien

  }else if(i > 2){
    // Solo cuando hay mas de dos ficheros se muestra el total
    showOutputWc("total", lflag, wflag, cflag);
  }
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

      otherwcFunc(cline);
    }else if((strcmp(cline[0],"findbysize") == 0) && (cline[1] != NULL) && (cline[2] != NULL)){
		findbysize(cline);
    }

     return 0;
}
