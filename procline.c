#include "smallsh.h"

/* El siguiente procedimiento procesa una l�nea de entrada. La ejecuci�n
 * termina cuando se encuentra un final de l�nea ('\n'). El car�cter ';'
 * sirve para separar unas �rdenes de otras y el car�cter '&' permite
 * una orden en background.
 */
void
procline(struct TokBuf * tb) {
    char * arg[MAXARG + 1];    /* Array de palabras: orden + argumentos. */
    TokType toktype;    /* Tipo de token. */
    int narg;        /* N�mero de argmentos leidos para la orden. */
    int where;        /* � El proceso se ejecutar� en primer o segundo plano ?.*/
    int ntoken;        /* Tokens procesados */

        narg = 0;
        for (ntoken = 0; ntoken < tb->length; ntoken++) {
            switch(toktype = tb->tokens[ntoken].type) {
                case ARG:   /* Se ha leido un argumento. */
                        if (narg < MAXARG)
                                arg[narg++] = tb->tokens[ntoken].data;
                        break;
                case EOL:
                case SEMICOLON:
                case AMPERSAND:

                case ARROBA:
                case AND:
                case OR:
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
                         * si se han introducido varias �rdenes
                         * separadas por ';' o '&'. */
                        narg = 0;
                        break;
                default:
                        ; /* Ignorar */
            }
        }
}

/* $Id: procline.c 1399 2007-12-20 09:45:07Z pedroe $ */