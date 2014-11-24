#include "smallsh.h"

#define MAX 120

/* Abre una tuberia en modo solo lectura y sustituye la salida
 * estandar por la tuberia recien abierta
 */
void openWritePipe(char * nombre_pipe){
    /* Abre la tuberia en modo solo escritura */
    int fd = open (nombre_pipe, O_WRONLY);

    /* Comprueba si se produce algun error */
    if (fd == -1){
        perror ("pipe_open_write");
        exit(EXIT_FAILURE); // Esta constante esta definida en stdio, y se usa por
                            // portabilidad, es igual a poner un 1-
    }
    close(1);   /*Cierra salida estandar actual*/
    dup(fd);
    close(fd);
}

/* Sustituye la entrada estandar por una tuberia de lectura */
void openReadPipe(int fdR){
    // Abrimos tuberia para lectura
    if (fdR == -1){
        perror ("pipe_open_read");
        exit(EXIT_FAILURE);
    }
    close(0);   /*Cierra entrada estandar actual*/
    dup(fdR);
    close(fdR);

}

/* Crea un fichero de tipo fifo para las tuberias con nombre con el
 * formato: fifo-sh-PID_DEL_PROCESO, dentro del directorio /tmp
 */
void createNamedPipe(char * nombre_pipe, int pid){
    /*Forma el nombre de la tuberia*/
    sprintf(nombre_pipe, "/tmp/fifo-sh-%i", pid);

    // Borramos las tuberias que si ya existen
    unlink(nombre_pipe);

    // Creacion de la tuberia de comunicacion con permisos rw-rw-rw
    if ((mkfifo(nombre_pipe, 006666) == -1) && (errno != EEXIST)){
        perror("Error al crear fifo");
        exit(EXIT_FAILURE);
    }
}

/* Recibe como parametros:
 * posInPipe : la posicion del comando que va a ejecutar dentro de los comandos concatenados con tuberias
 * typepipe: tipo de tuberia (NAMED y UNNAMED)
 * fd: array con descriptores de fichero para tuberias sin nombre
 * nombre_pipe: array donde almacenar el nombre de la tuberia con nombre
 * fdR: descriptor de fichero donde leer la entrada en caso de no ser el primer comando de la tuberia.
 */
void gestionaPipes(int posInPipe, int typepipe, int fd[], char * nombre_pipe, int fdR){

    if(posInPipe != 0){     /*  si no es primer comando, lee de la pipe */
        openReadPipe(fdR);  /*  La sustitucion de la entrada estandar
                                por la tuberia de lectura
                                es igual en los dos tipos de tuberias */
    }

    if (posInPipe != -1){   /* si no es ultimo comando escribe en la pipe */
        if(typepipe == NAMEDPIPE){
            sprintf(nombre_pipe, "/tmp/fifo-sh-%i", getpid());
            openWritePipe(nombre_pipe);
        }else{
            close(1);       /* Si es tuberia sin nombre no hay que crear nada, */
            dup(fd[1]);     /* se sustituye solamente la salida estandar por */
            close(fd[1]);   /* la tuberia de escritura*/
        }
    }

}

int getReadFd(int fd[], int typepipe, char * nombre_pipe){
    if(typepipe == NAMEDPIPE)

        return open(nombre_pipe, O_RDONLY);  /* Da igual que sea bloqueante porque
                                                     si lee, es porque alguien va a estar
                                                     escribiendo ya */
    else{         /* Se trata de una tuberia sin nombre */
        close(fd[1]);   /* Se cierra descriptor de escritura de la tuberia */
        return fd[0];
    }
}


/* Ejecuta una orden. "cline" es una array de palabras que contiene el nombre
 * de la orden y los parámetros de dicha orden. "where" indica si dicha
 * orden se debe ejecutar en primer o segundo plano.
 */
int
runcommand(char **cline, int where) {
        int pid, exitstat, ret;

        if ((pid = fork()) < 0) {
                perror("Error al realizar fork");
                exit(EXIT_FAILURE);
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
                exit(EXIT_FAILURE);
            }
        }

        /* Creacion del hijo */
        if ((pid = fork()) < 0) {

                perror("Error al realizar fork");
                exit(EXIT_FAILURE);

        } else if(pid > 0){ /* PADRE - crea la tuberia si es con nombre */

                if(typepipe == NAMEDPIPE){
                    createNamedPipe(nombre_pipe, pid);
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

                if(posInPipe != -1){    /* No es el ultimo comando prepara el descriptor de lectura que retorna la funcion */
                    fd_return = getReadFd(fd, typepipe, nombre_pipe);
                } else { /* El padre espera al ultimo proceso */
                    while ((ret = wait(&exitstat)) != pid && ret != -1);
                }


        } else {   /* HIJO */
                /* Establece comportamiento para el hijo */
                signal(SIGINT,SIG_DFL);
                signal(SIGQUIT,SIG_DFL);

                /* Modifica la tabla de descriptores segun posicion del comando a ejecutar y tipo de tuberia*/
                gestionaPipes(posInPipe, typepipe, fd, nombre_pipe, fdR);

                // Ejecución
                execvp(*cline, cline);    /* Ejecutamos la orden. */
                perror(*cline);           /* Se llega aquí si no se ha podido
                                             ejecutar. Por tanto, se ha producido
                                             un error.*/
                exit(127);

        }

        return(fd_return);

}


/* Limpia todas las tuberias con nombre creadas durante la ejecucion
 * de un conjunto de ordenes concatenadas por tuberias
 */
void cleanNamedPipes(){
    DIR *dir = opendir("/tmp"); //obtenemos el descriptor de directorio

    if(dir == NULL){
        perror("Error al abrir el directorio tmp");
        exit(EXIT_FAILURE);
    }

    errno = 0;      // se establece a 0, por si hubiese modificado anteriormente

    struct dirent *entry;
    while(( entry = readdir(dir)) != NULL ) {
        char * isPipe = "fifo-sh-";
        char checkPipe[9]; // long(fifo-sh-) = 8
        char removePipe[16];
        /* copia los 8 primeros caracteres del fichero del directorio tmp
         * para despues comprobar si es un fichero de pipe
         */
        strncpy(checkPipe,entry->d_name,8);
        checkPipe[8] = '\0';
        if(strcmp(checkPipe, isPipe) == 0 ){    /*son iguales*/
            sprintf(removePipe, "/tmp/%s", entry->d_name);
            unlink(removePipe);  /* se elimina el ficheor */
        }
    }

    if (errno) { // se añade porque tiene un tratamiento distinto al manejo de errores de antes
        perror("Error al limpiar pipes del directorio temporal");
        exit(EXIT_FAILURE);
    }

    if(closedir(dir) == -1){
        perror("Error al cerrar directorio tmp");
        exit(EXIT_FAILURE);
    }
}