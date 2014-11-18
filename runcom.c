#include "smallsh.h"

#define MAX 128
#define DEBUG 0

void closefd(int pos, int fd){
    close(pos);
    dup(fd);
    close(fd);
}


void openWritePipe(char * nombre_pipe){

    /* Hijo solo crea nombre de su tuberia si tiene que escribir en ella*/
    sprintf(nombre_pipe, "/tmp/fifo-sh-%i", getpid());

    /* Abre la tuberia en modo solo escritura */
               // Entero descriptor de fichero
    int fd = open (nombre_pipe, O_WRONLY);

    /* Comprueba si se produce algun error */
    if (fd == -1){
        perror ("pipe_open_write");
        exit(EXIT_FAILURE); // Esta constante esta definida en stdio, y se usa por
                            // portabilidad, es igual a poner un 1-
    }

    closefd(1,fd); /*Cierra salida estandar actual*/

}

void openReadPipe(char * pipeToRead){

    // Solo para comandos intermedios se lee de la entrada del nuevo descriptor
    // Abrimos tuberia para lectura
    int fdR = open (pipeToRead, O_RDONLY );
    if (fdR == -1){
        perror ("pipe_open_read");
        exit(EXIT_FAILURE); // Esta constante esta definida en stdio, y se usa por
                            // portabilidad, es igual a poner un 1-
    }

    closefd(0, fdR); /*Cierra entrada estandar actual*/

}

/* Ejecuta una orden. "cline" es una array de palabras que contiene el nombre
 * de la orden y los parámetros de dicha orden. "where" indica si dicha
 * orden se debe ejecutar en primer o segundo plano.
 */

int
runcommand(char **cline, int where) {
        int pid, exitstat, ret;

        if ((pid = fork()) < 0) {
                perror("smallsh");
                return(-1);
        }

        if (pid == 0) {                   /* Estamos en el hijo. */

        		/*Nuevo, establecemos comportamiento para el hijo*/
        		signal(SIGINT,SIG_DFL);
				signal(SIGQUIT,SIG_DFL);

                execvp(*cline, cline);    /* Ejecutamos la orden. */
                perror(*cline);           /* Se llega aquí si no se ha podido
                                             ejecutar. Por tanto, se ha producido
                                             un error.*/
                exit(127);
        }

        /* Estamos en el padre. Si la orden se ejecuta en segundo plano, no
         * debemos esperar por lo que mostramos el pid del nuevo proceso y
         * regresamos. */
        if (where == BACKGROUND) {
        		insert(pid, *cline, procList);/*Nuevo, añadimos a la lista de procesos en 2º plano.*/
        		setColor(COLOR_BACK_BLUE);
                printf("[Identificador de proceso: %d]",pid);
                setColor(COLOR_RESET);
                printf("\n");
                return(0);
        }

        /* Si la orden se ejecuta en primer plano, debemos esperar a que
         * termine.*/
        while ((ret = wait(&exitstat)) != pid && ret != -1)
        ;

        return(ret == -1 ? -1 : exitstat);
}


/*nueva funcion igual al runcom pero que devuelve salida con descriptor de fichero*/
/* retorna el nombre del pipe donde debe leer el siguiente comando*/
char *
runcommandPipe(char **cline, int where, int typepipe, int posInPipe, char * pipeToRead) {
        // static para poder retornar la cadena
        static char nombre_pipe[MAX];

        int pid, ret, exitstat;

        if ((pid = fork()) < 0) {
                perror("smallsh");
                return(NULL);
        }

        if(pid > 0 && posInPipe != -1){ /* Estamos en el padre y no es el ultimo comando*/

            // Creamos nombre de la tuberia tras crear el hijo
            // El hijo conocera la tuberia porque puede saber su pid
            sprintf(nombre_pipe, "/tmp/fifo-sh-%i", pid);

            // Borramos las tuberias que pudieran existir
            unlink(nombre_pipe);

            // Creacion de la tuberia de comunicacion con permisos rw-rw-rw
            if ((mkfifo(nombre_pipe, 006666) == -1) && (errno != EEXIST)){
                perror(nombre_pipe);
                exit(EXIT_FAILURE);
            }

        }

        if(pid == 0){                  /* Estamos en el hijo. */

            /*Nuevo, establecemos comportamiento para el hijo*/
            signal(SIGINT,SIG_DFL);
            signal(SIGQUIT,SIG_DFL);

            /* Si no es el primer comando, se cambia la entrada estandar por la tuberia */
            if(posInPipe != 0){
                openReadPipe(pipeToRead);
            }

            // Abrimos tuberia para escritura si no es el ultimo comando y se cambia la tuberia por la salida estandar */
            if(posInPipe != -1){
                openWritePipe(nombre_pipe);
            }

            if(posInPipe == 0){ /* primer comando de la tuberia */

                execvp(*cline, cline);    /* Ejecutamos la orden. */
                perror(*cline);           /* Se llega aquí si no se ha podido
                                             ejecutar. Por tanto, se ha producido
                                             un error.*/
                exit(127);
            }else{

                execvp(*cline, cline);    /* Ejecutamos la orden. */
                perror(*cline);           /* Se llega aquí si no se ha podido
                                             ejecutar. Por tanto, se ha producido
                                             un error.*/
                exit(127);
            }
        }

        /* Estamos en el padre. Si la orden se ejecuta en segundo plano, no
         * debemos esperar por lo que mostramos el pid del nuevo proceso y
         * regresamos. */
        if (where == BACKGROUND) {
                insert(pid, *cline, procList);/*Nuevo, añadimos a la lista de procesos en 2º plano.*/
                setColor(COLOR_BACK_BLUE);
                printf("[Identificador de proceso: %d]",pid);
                setColor(COLOR_RESET);
                printf("\n");
                return(0);
        }

        /* Si la orden se ejecuta en primer plano y es la última en caso de formar parte
         * de una tuberia, debemos esperar a que termine.*/
        /* si es el ultimo comando de la tuberia el padre espera a que termine */
        if(posInPipe == -1)
            while ((ret = wait(&exitstat)) != pid && ret != -1);

        /* NUEVO: retorna NULL si hay fallo, o el nombre de la pipa donde la siguiente instruccion debe leer*/
        return(nombre_pipe);
}
/* $Id: runcom.c 1399 2007-12-20 09:45:07Z pedroe $ */
