#ifndef __USERFN_H
#define __USERFN_H

enum
{
    FLECHA_ARRIBA,
    FLECHA_ABAJO
};

/**
 * user_inicializar
 *
 * El shell llama a esta función al principio para que se realicen
 * las acciones de inicialización necesarias.
 */
void user_inicializar(void);

/**
 * user_finalizar
 *
 * El shell llama a esta función al final para que se realicen
 * las acciones de finalización necesarias.
 */
void user_finalizar(void);


/**
 * user_getPrompt
 *
 * Devuelve una cadena con el prompt a mostrar. La cadena la debe reservar
 * esta función con malloc.
 *
 * @return cadena de prompt
 */
char * user_getPrompt(void);

/**
 * user_flecha
 *
 * Devuelve la cadena que se debe mostrar en la línea de órdenes al pulsar
 * la flecha arriba o la flecha abajo.
 *
 * @param direccion indica si es flecha arriba o flecha abajo
 * @param patron patrón a buscar en la historia (anterior o posterior, según
 *               el valor de "dirección"
 *
 * @return cadena a poner en la línea de órdenes
 */
char * user_flecha(int direccion, char* patron);

/**
 * user_nueva_orden
 *
 * Esta función es llamada cada vez que el usuario pulsa INTRO, e informa
 * de la orden que ha escrito el usuario. Esta función debe copiar la cadena
 * y no modificar la cadena que se le pasa.
 *
 * @param orden La orden que ha escrito el usuario
 *
 */
void user_nueva_orden(char * orden);

/**
 * user_tabulador
 *
 * Devuelve (si procede) la cadena a añadir al pulsar el tabulador. La
 * función recibe en "número" si el tabulador se ha pulsado para una orden
 * (un 1) o si se ha pulsado para un argumento (un 2). El argumento "numtab"
 * especifica el número de veces que se ha pulsado el tabulador (1 ó 2).
 * En la primera pulsación la función completará si sólo hay una opción. Si
 * no, emitirá un pitido. En la segunda pulsación, si hay varias posibilidades,
 * se listarán todas ellas.
 *
 * @param parte parte de la cadena a la que se aplica el tabulador
 * @param numero número del argumento: 1->orden, 2->argumento
 * @param numtab número de veces que se ha pulsado el tabulador (1 ó 2)
 *
 * @return Parte restante de la cadena completada o NULL si no se puede
 *         completar
 */
char * user_tabulador(char * parte, int numero, int numtab);

#endif

/* $Id: userfn.h 1399 2007-12-20 09:45:07Z pedroe $ */
