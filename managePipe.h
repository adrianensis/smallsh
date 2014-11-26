#ifndef __FINDBYSIZE_H
#define __FINDBYSIZE_H

void openWritePipe(char * nombre_pipe);
void openReadPipe(int fdR);
void createNamedPipe(char * nombre_pipe, int pid);
void gestionaPipes(int posInPipe, int typepipe, int fd[], char * nombre_pipe, int fdR);
int getReadFd(int fd[], int typepipe, char * nombre_pipe);
void cleanNamedPipes();

#endif