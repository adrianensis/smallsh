#ifndef __FLEUTIL_H
#define __FLEUTIL_H

struct stat getStat(int fileDesc);

int isDir(char* name);
int isReg(char* name);
int isExe(char* name);

#endif
