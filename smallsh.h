#ifndef __SMALLSH_H
#define __SMALLSH_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

/* Definición del buffer de tokens */
#include "gettok.h"

/* Funciones del usuario */
#include "userfn.h"

/* Funciones del buffer */
#include "buffer.h"

/*Nuevas librerias incluidas*/
#include <time.h> // para time_t
#include <errno.h> // para errno
#include "color.h"
#include "plist.h"
#include "findbysize.h"
#include "fileutil.h"
#include "managePipe.h"
#include "otherwc.h"
#include "trietree.h"
#include "strlist.h"
#include <dirent.h>
#include <sys/stat.h>
#include <fcntl.h>

/* Estructura para almacenar instrucciones */
//#include "sortedArrayMap.h"

#define MAXARG 512

#define FOREGROUND 0
#define BACKGROUND 1

/*Nuevas macros para las tuberias*/
#define NAMEDPIPE 2
#define UNNAMEDPIPE 3

/*Nueva macro para gestion de errores*/
#define EXIT_ERROR -2

/*Nuevas Variables globales*/
char* currentDirectory;
char* initDirectory;
time_t timeSeconds;
char* promptStr; // Prompt, lo pongo global para liberarlo en user finalizar.
plist* procList;
trieTree* dictionary;
strlist* cmdHistory;

/*Funciones*/
void procline(struct TokBuf*);
int runcommand(char **, int);

/*Nueva funcion sobrecargada para tuberias*/
int runcommandPipe(char **, int, int, int, int);

/*Nuevas Funciones*/
int isInternal(char*);
int runinternal(char **);

#endif

/* $Id: smallsh.h 1398 2007-12-20 08:06:01Z pedroe $ */
