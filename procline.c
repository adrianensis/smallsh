#include "smallsh.h"

/* El siguiente procedimiento procesa una línea de entrada. La ejecución
 * termina cuando se encuentra un final de línea ('\n'). El carácter ';'
 * sirve para separar unas órdenes de otras y el carácter '&' permite
 * una orden en background.
 */
void
procline(struct TokBuf * tb) {
    char * arg[MAXARG + 1];    /* Array de palabras: orden + argumentos. */
    TokType toktype;    /* Tipo de token. */
    int narg;        /* Número de argmentos leidos para la orden. */
    int where;        /* ¿ El proceso se ejecutará en primer o segundo plano ?.*/
    int ntoken;        /* Tokens procesados */
    
    /*NUEVO*/
    char** pipeArg[MAXPIPES];
    int pipe;
	pipe = 0;
	
	
	
        narg = 0;
        for (ntoken = 0; ntoken < tb->length; ntoken++) {
            switch(toktype = tb->tokens[ntoken].type) {
                case ARG:   /* Se ha leido un argumento. */
                        if (narg < MAXARG)
                                arg[narg++] = tb->tokens[ntoken].data;
                        break;
                case PIPE:
		            	/*Que hacemos cuando nos encontramos | .
		            	Todos los argumentos que llevamos hasta el momento se acumulan.*/
		            	
		            	/*Si encontramos | y hay argumentos, se meten.*/
		            	/* Por el contrario estariamos hablando de un caso como este, por ejemplo: " <vacio> | COMANDO | ... " */
		            	if(narg > 0){
		            		pipeArg[pipe++] = arg;
		            		narg = 0;
		            	}
		            		
		            	break;
                case EOL:
                case SEMICOLON:
                case AMPERSAND:
                case ARROBA:
                case AND:
                case OR:
                		if(pipe == 0){ /*Nuevo - Aqui entramos cuando pulsamos EOL o ; o etc... y NO estamos usando pipes.*/
		                    where = (toktype == AMPERSAND) ? BACKGROUND :
		                        FOREGROUND;
		                    if (narg != 0) {
		                            arg[narg] = NULL; /*El ultimo se establece NULL*/
		                            
		                            if(isInternal(arg[0]))
		                            	runinternal(arg); // nuevo
		                            else
		                            	runcommand(arg, where);
		                    }
		                    /* Seguir con la siguiente orden. Esto se da
		                     * si se han introducido varias órdenes
		                     * separadas por ';' o '&'. */
		                    narg = 0;
		                }else { /*Nuevo - Aqui entramos cuando pulsamos EOL o ; o etc... y SI estamos usando pipes.*/
		                	// TODO hacer esto
		                }
                        break;
                        
                default:
                        ; /* Ignorar */
            }
        }
}

/* $Id: procline.c 1399 2007-12-20 09:45:07Z pedroe $ */
