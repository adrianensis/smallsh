#include "smallsh.h"

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