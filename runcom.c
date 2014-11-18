#include "smallsh.h"

#define MAX 128

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
runcommandPipe(char **cline, int where, int typepipe, int readOutput, char * pipeToRead) {
        // static para poder retornar la cadena
        static char nombre_pipe[MAX];

        int pid, ret, exitstat;

        printf("Pid: %i - pipa: aun por crear - comando a ejecutar: %s\n", getpid(), cline[0]);

        if ((pid = fork()) < 0) {
                perror("smallsh");
                return(NULL);
        }

        if(pid > 0 && typepipe != -1){
            //Creamos nombre de la tuberia tras crear el hijo
            sprintf(nombre_pipe, "/tmp/fifo-sh-%i", pid);

            // Borramos las tuberias que pudieran existir
            unlink(nombre_pipe);

            // Creacion de la tuberia de comunicacion con permisos rw-rw-rw
            if ((mkfifo(nombre_pipe, 006666) == -1) && (errno != EEXIST)){
                perror(nombre_pipe);
                exit(EXIT_FAILURE);
            }
        }
        if (pid == 0) {                   /* Estamos en el hijo. */
                printf("Soy HIJO de %i con pid: %i - pipa termina en : %i - comando a ejecutar: %s\n", getppid(), getpid(), getpid(), cline[0]);
                /*Nuevo, establecemos comportamiento para el hijo*/
                signal(SIGINT,SIG_DFL);
                signal(SIGQUIT,SIG_DFL);

                int fd;
                // Abrimos tuberia para escritura si no es solo de lectura
                // typepipe == -1 --> solo lectura
                if(typepipe == 0){
                    printf("Comienzo a abrir pipa para escribir...\n");
                    /* hijo solo crea nombre de su tuberia si tiene que escribir en ella*/
                    sprintf(nombre_pipe, "/tmp/fifo-sh-%i", getpid());
                    fd = open (nombre_pipe, O_WRONLY);
                    printf("termino de abrir pipe para escritura\n");
                    if (fd == -1){
                        printf("error al abrir pipe\n");
                        perror ("pipe_open_write");
                        exit(EXIT_FAILURE); // Esta constante esta definida en stdio, y se usa por
                                            // portabilidad, es igual a poner un 1-
                    }
                    printf("HIJO pid: %i --> abro pipa de escritura bien: %s\n", getpid(), nombre_pipe);
                    printf("entra en output == 0 - va a escribir la salida en pipa\n");
                    close(1);   /*Cierra salida estandar actual*/
                    dup(fd);    /*Define como salida estandar la tuberia*/
                    close(fd);  /*Cierra el descriptor de la tuberia*/
                }

                if(readOutput == 0){
                    printf("soy %i y entro en: readOutput == 0", getpid());
                    // printf("entra en output == 0 - va a escribir la salida en pipa\n");
                    // close(1);   /*Cierra salida estandar actual*/
                    // dup(fd);    /*Define como salida estandar la tuberia*/
                    // close(fd);  /*Cierra el descriptor de la tuberia*/
                    execvp(*cline, cline);    /* Ejecutamos la orden. */
                    perror(*cline);           /* Se llega aquí si no se ha podido
                                                 ejecutar. Por tanto, se ha producido
                                                 un error.*/
                    exit(127);
                }else{
                    printf("%i entra en output == 1\n", getpid());
                    // Solo para comandos intermedios se lee de la entrada del nuevo descriptor
                    // Abrimos tuberia para lectura
                    printf("Comienzo a abrir pipa para leer...\n");
                    int fdR = open (pipeToRead, O_RDONLY );
                    if (fdR == -1){
                        perror ("pipe_open_read");
                        exit(EXIT_FAILURE); // Esta constante esta definida en stdio, y se usa por
                                            // portabilidad, es igual a poner un 1-
                    }

                    printf("pid: %i - cierro entrada estandar para leer de la pipa anterior\n", getpid());
                    close(0);   /*Cierra entrada estandar actual*/
                    dup(fdR);    /*Define como entrada estandar la tuberia*/
                    close(fdR);  /*Cierra el descriptor de la tuberia*/

                    if(typepipe == 1){
                        printf("Comienzo a abrir pipa para escribir...\n");
                        /* hijo solo crea nombre de su tuberia si tiene que escribir en ella*/
                        sprintf(nombre_pipe, "/tmp/fifo-sh-%i", getpid());
                        fd = open (nombre_pipe, O_WRONLY);
                        printf("termino de abrir pipe para escritura\n");
                        if (fd == -1){
                            printf("error al abrir pipe\n");
                            perror ("pipe_open_write");
                            exit(EXIT_FAILURE); // Esta constante esta definida en stdio, y se usa por
                                                // portabilidad, es igual a poner un 1-
                        }
                        printf("HIJO pid: %i --> abro pipa de escritura bien: %s\n", getpid(), nombre_pipe);
                        printf("entra en output == 0 - va a escribir la salida en pipa\n");
                        close(1);   /*Cierra salida estandar actual*/
                        dup(fd);    /*Define como salida estandar la tuberia*/
                        close(fd);  /*Cierra el descriptor de la tuberia*/
                    }

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
        if(readOutput == 1 && typepipe == -1)
            while ((ret = wait(&exitstat)) != pid && ret != -1);

        /* NUEVO: retorna NULL si hay fallo, o el nombre de la pipa donde la siguiente instruccion debe leer*/
        return(nombre_pipe);
}
/* $Id: runcom.c 1399 2007-12-20 09:45:07Z pedroe $ */
