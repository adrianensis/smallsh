#ifndef __MANAGEPIPE_H
#define __MANAGEPIPE_H

int openWritePipe(char * nombre_pipe);
int openReadPipe(int fdR);
int createNamedPipe(char * nombre_pipe, int pid);
int gestionaPipes(int posInPipe, int typepipe, int fd[], char * nombre_pipe, int fdR);
int getReadFd(int fd[], int typepipe, char * nombre_pipe);
int cleanNamedPipes();

#endif
