#include "smallsh.h"

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

/* $Id: runcom.c 1399 2007-12-20 09:45:07Z pedroe $ */
