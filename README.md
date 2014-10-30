# Práctica de ASO Smallsh

Como guía rápida podemos decir:

* Los ficheros que hemos creado son:
>* runinternal.c
>* plist.h
>* plist.c

* Los ficheros que hemos modificado son:
>* runcom.c para establecer el comportamiento de señales del hijo a crear.
>* smallsh.c para establecer el comportamiento y tratamiento de señales y alarma del padre (smallsh, que hace de bash y crea hijos con fork-exec).
>* smallsh.h para incluir nuevas librerias y funciones.
>* userfn.c para inicializar y personalizar al usuario.