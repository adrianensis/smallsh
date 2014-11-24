#ifndef __COLOR_H
#define __COLOR_H

#include <stdio.h>

/*COLORES PARA LA SALIDA*/
#define COLOR_BLACK   "\033[30m"
#define COLOR_RED     "\033[31m"
#define COLOR_GREEN   "\033[32m"
#define COLOR_YELLOW  "\033[33m"
#define COLOR_BLUE    "\033[34m"
#define COLOR_MAGENTA "\033[35m"
#define COLOR_CYAN    "\033[36m"
#define COLOR_WHITE    "\033[37m"

#define COLOR_BOLD_BLACK   "\033[1;30m"
#define COLOR_BOLD_RED     "\033[1;31m"
#define COLOR_BOLD_GREEN   "\033[1;32m"
#define COLOR_BOLD_YELLOW  "\033[1;33m"
#define COLOR_BOLD_BLUE    "\033[1;34m"
#define COLOR_BOLD_MAGENTA "\033[1;35m"
#define COLOR_BOLD_CYAN    "\033[1;36m"
#define COLOR_BOLD_WHITE    "\033[1;37m"

#define COLOR_BACK_BLACK "\033[037;40m"
#define COLOR_BACK_RED "\033[037;41m"
#define COLOR_BACK_GREEN "\033[037;42m"
#define COLOR_BACK_YELLOW "\033[037;43m"
#define COLOR_BACK_BLUE "\033[037;44m"
#define COLOR_BACK_MAGENTA "\033[037;45m"
#define COLOR_BACK_CYAN "\033[037;46m"
#define COLOR_BACK_WHITE "\033[030;47m"

#define COLOR_RESET   "\x1b[0;0m"

// FUNCIONES PARA COLORES
void setColor(char*);

#endif
