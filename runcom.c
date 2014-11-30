#include "smallsh.h"

#define MAX 120


/* Ejecuta una orden. "cline" es una array de palabras que contiene el nombre
 * de la orden y los parámetros de dicha orden. "where" indica si dicha
 * orden se debe ejecutar en primer o segundo plano.
 */
int
runcommand(char **cline, int where) {
        int pid, exitstat, ret;

        if ((pid = fork()) < 0) {
                perror("Error al realizar fork");
                return(EXIT_ERROR);
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


/* Ejecuta una orden y redirige su salida en funcion del tipo de tuberia.
 * "cline" es una array de palabras que contiene el nombre
 * de la orden y los parámetros de dicha orden. "where" indica si dicha
 * orden se debe ejecutar en primer o segundo plano.
 * "typepipe" indica el tipo de tuberia usada (con nombre "%" o sin nombre "|")
 * "posInPipe" indica la posicion de la orden a ejecutar en el conjunto de
 * ordenes de la tuberia y "fdR" indica el descriptor de fichero de donde leer
 * entrada si fuese necesario.
 */
int runcommandPipe(char **cline, int where, int typepipe, int posInPipe, int fdR){
        /* Array para formar el nombre de la tuberia con nombre a crear */
        static char nombre_pipe[MAX];

        int pid, ret, exitstat;

        int fd[2];
        int fd_return;

        /* Crea la tuberia sin nombre solo si no es ultimo comando */
        if(typepipe == UNNAMEDPIPE && posInPipe != -1){
            if(pipe(fd) == -1 ){
                perror("Error al intentar crear pipe");
                return(EXIT_ERROR);
            }
        }

        /* Creacion del hijo */
        if ((pid = fork()) < 0) {

                perror("Error al realizar fork");
                return(EXIT_ERROR);

        } else if(pid > 0){ /* PADRE - crea la tuberia si es con nombre */

                if(typepipe == NAMEDPIPE){
                    if(createNamedPipe(nombre_pipe, pid) == EXIT_ERROR){
                        return(EXIT_ERROR); //Propaga el error
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

                if(posInPipe != -1){    /* Si no es el ultimo comando prepara el descriptor de lectura que retorna la funcion */
                    fd_return = getReadFd(fd, typepipe, nombre_pipe);
                } else { /* El padre espera al ultimo proceso */
                    while ((ret = wait(&exitstat)) != pid && ret != -1);
                }


        } else {   /* HIJO */
                /* Establece comportamiento para el hijo */
                signal(SIGINT,SIG_DFL);
                signal(SIGQUIT,SIG_DFL);

                /* Modifica la tabla de descriptores segun posicion del comando a ejecutar y tipo de tuberia*/
                if( gestionaPipes(posInPipe, typepipe, fd, nombre_pipe, fdR) == EXIT_ERROR ){
                    return(EXIT_ERROR); //Propaga el error
                }

                // Ejecución
                execvp(*cline, cline);    /* Ejecutamos la orden. */
                perror(*cline);           /* Se llega aquí si no se ha podido
                                             ejecutar. Por tanto, se ha producido
                                             un error.*/
                exit(127);

        }

        return(fd_return);

}