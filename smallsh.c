#include "smallsh.h"


// nuevo, tratamiento ante hijo muerto.
void deadSon(int nsig, siginfo_t* info, void* nothing){
	if(find(info->si_pid, procList) != NULL){
		int exitstat;
		printf("\n");
		setColor(COLOR_BACK_GREEN);
		// %c, 164 imprime la "ñ"
		printf("Proceso en segundo plano terminado, recibida signal %d:\n[%d] ha retornado:", SIGCHLD, info->si_pid);

		/* Esperar al proceso hijo (obligatorio para que no quede Zombie) */
		wait (&exitstat);
		printf(" %d", exitstat);
		setColor(COLOR_RESET);
		printf("\n");
		erase(info->si_pid, procList);
	}
}

// Nuevo llamada al demonio de la lista de procesos.
void runDaemon(int nothing){
	alarmDaemon(procList);
}

int
main(int argc, char **argv)
{
		/*
		NUEVO
		ESTABLECEMOS EL COMPORTAMIENTO ANTE SEÑALES
		*/
		struct sigaction action;
		sigset_t set;

		sigemptyset(&set);
		sigaddset(&set,SIGCHLD);
		action.sa_sigaction=deadSon;
		action.sa_mask=set;
		action.sa_flags=SA_RESTART | SA_SIGINFO;
		action.sa_restorer=NULL;

		/* Activa el tratamiento de SIGCHLD */
		sigaction(SIGCHLD,&action,(void*)NULL);

		signal(SIGINT,SIG_IGN);
		signal(SIGQUIT,SIG_IGN);

		signal(SIGALRM, runDaemon);

		// Arrancamos el demonio
		runDaemon(0);
		alarm(1);


        struct TokBuf *tb;
        struct Buffer *buf;

        /* Llamar a la función de inicialización del shell */
        user_inicializar();

        /* Establecer modo no interpretado en la entrada */
        modoInterpretado(0, 0);

        /* Procesar órdenes */
        /*nuevo, añadido exit*/
        int exit = 0;
        while (exit==0 && ((buf = userin()) != NULL))
        {
        		/*nuevo*/
        		if(strcmp(buf->data, "exit") == 0)
        			exit = 1;

                tb = gettok(buf->data);

                procline(tb);

                /* Liberar el TokBuf que creó la llamada a userin() */
                liberaTokBuf(tb);
                free (tb);
                liberaBuffer(buf);
                free (buf);

        }

        /* Restaurar el modo de la entrada */
        modoInterpretado(0, 1);

        /* Finalmente, a la función de finalización */
        user_finalizar();

        /* Retornar una salida sin error */
        return 0;
}

/* $Id: smallsh.c 1398 2007-12-20 08:06:01Z pedroe $ */
