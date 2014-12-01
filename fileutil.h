#ifndef __FLEUTIL_H
#define __FLEUTIL_H

/*#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <dirent.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>*/



struct stat getStat(int fileDesc);

int isDir(char* name);
int isReg(char* name);
int isExe(char* name);

#endif
