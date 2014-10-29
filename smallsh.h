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
#include "color.h"
#include "plist.h"

#define MAXARG 512

#define FOREGROUND 0
#define BACKGROUND 1

/*Variables globales*/
char* currentDirectory;
char* initDirectory;
time_t timeSeconds;
char* promptStr; // Prompt, lo pongo global para liberarlo en user finalizar.
plist* procList;

/*Funciones*/
void procline(struct TokBuf*);
int runcommand(char **, int);

/*Nuevas Funciones*/
int isInternal(char*);
int runinternal(char **);

#endif

/* $Id: smallsh.h 1398 2007-12-20 08:06:01Z pedroe $ */
