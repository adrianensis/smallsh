#ifndef __OTHERWC_H
#define __OTHERWC_H

int otherwc(char** cline);
void showOutputWc(char *file, int lflag, int wflag, int cflag);
void countChar(char c);
int getWord(char buf[], int offset, int sizeBuffer);
int countFile(char *file, int lflag, int wflag, int cflag);

#endif
