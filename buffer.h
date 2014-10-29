#ifndef __BUFFER_H
#define __BUFFER_H

struct Buffer
{
    int    len;
    int    max;
    char    * data;
};

/* Crea un buffer vacío */
struct Buffer * creaBuffer (char* buf);

/* Inicia un buffer vacío */
void iniciaBuffer(struct Buffer* b, char* buf);

/* Elimina un buffer */
void liberaBuffer(struct Buffer * b);

/* Añade un carácter al final */
struct Buffer * anyadeChar(struct Buffer * b, char c);

/* Añade una cadena al final */
struct Buffer * anyadeCadena(struct Buffer * b, char * c);

/* Elimina el último carácter del final */
struct Buffer * eliminaUltimo(struct Buffer * b);

#endif

/* $Id: buffer.h 1399 2007-12-20 09:45:07Z pedroe $ */
