#include "smallsh.h"
#include <ctype.h>

// Variables contadoras parciales
int charCounter = 0;
int wordCounter = 0;
int lineCounter = 0;

// variables contadoras totales
int totalCharCounter;
int totalWordCounter;
int totalLineCounter;

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
// fuera de smallsh pero eso no es fallo nuestro
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

void countFile(char *file, int lflag, int wflag, int cflag){
  // abre el fichero
  int fd = open (file, O_RDONLY );

  if (fd == -1){
    perror ("Error al abrir fichero para lectura");
        /* Restaurar el modo de la entrada */
        modoInterpretado(0, 1);

        /* Finalmente, a la función de finalización */
        user_finalizar();
    exit(EXIT_FAILURE);
  }

  // se obtiene su estado
  struct stat st;

  fstat(fd, &st);

  int tam_buf = st.st_size;
  char buf[tam_buf];
  int r;

  while((r=read(fd, buf, tam_buf)) > 0); // lee todo el fichero

  // Se cierra el fichero
  if (close(fd) == -1){
    perror("Error al cerrar fichero de lectura");
    exit(EXIT_FAILURE);
  }

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
void otherwc(char **cline){

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

      countFile(cline[i], lflag, wflag, cflag);
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