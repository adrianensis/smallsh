#ifndef __FINDBYSIZE_H
#define __FINDBYSIZE_H

#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <dirent.h>

// argumentos: tamaño maximo, tamaño minimo, array de directorios.
void findbysize(int max, int min, int numDirs, char** dirs);

#endif
