#include "smallsh.h"

#define DEBUG 0
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

    // provisional a falta de saber si es asi
    //---------------------------------------
    int typepipe = 0;
    int posInPipe = 0;  /*              command1 % command2 % ... % commandN    */
                        /* posInPipe =     0           1                -1      */
    int readOutput = 0;
    //fileDesc_t lastOutPipe;
    int fdR = -1;

    // estructura que contiene la operacion de pipe (nombre o sin nombre) y el conjunto de argumentos a ejecutar
    // struct tokToJoin{
    //     char pipe;
    //     char * arg[MAXARG+1];
    // };

    // struct tokToJoin * joinBuf;     // array de instrucciones a unir
    //__________________

    // a | b % c, cuando se lea el pipe, se pasa a, al runcom indicando el tpo de pipe y se devuelve el descriptor de fichero
    // luego b al leer % se ejecuta y sabe que tiene que leer del descriptor y escribir en un fichero para que lo lea el siguiente %

   // char **joinBuf; /* Array de arrays de ordenes, almacena conjuntos de ordenes + argumentos */


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
                                if(readOutput > 0){
                                    posInPipe = -1;
                                    fdR = runcommandPipe(arg, where, typepipe, posInPipe, fdR);
                                    cleanNamedPipes();
                                }else if(isInternal(arg[0])){
                                        runinternal(arg); // nuevo
                                }else{
                                        runcommand(arg, where);
                                }
                        }

                        /* Seguir con la siguiente orden. Esto se da
                         * si se han introducido varias órdenes
                         * separadas por ';' o '&'. */
                        narg = 0;
                        break;
                case PORCIENTO:
                case PIPE:
                        typepipe = (toktype == PORCIENTO) ? NAMEDPIPE : UNNAMEDPIPE;

                       // where = (toktype == AMPERSAND) ? BACKGROUND :
                        //    FOREGROUND;
                        if (narg != 0) {
                                arg[narg] = NULL; /*El ultimo se establece NULL*/
                                fdR = runcommandPipe(arg, where, typepipe, posInPipe, fdR);
                                posInPipe = 1; // es un comando de los que leen y escriben
                                readOutput = 1; /*Establece que el siguiente debe leer la tuberia*/
                        }
                        /* Seguir con la siguiente orden. Esto se da
                         * si se han introducido varias órdenes
                         * separadas por ';' o '&'. */
                        narg = 0;
                        break;
                default:
                        ; /* Ignorar */
            }
        }
}

/* $Id: procline.c 1399 2007-12-20 09:45:07Z pedroe $ */
