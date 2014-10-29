%{
#include "gettok.h"
%}

%%

'               trataToken( QUOTE, yytext );
&               trataToken( AMPERSAND, yytext );
;               trataToken( SEMICOLON, yytext );
\|\|            trataToken( OR, yytext );
&&              trataToken( AND, yytext );
\|              trataToken( PIPE, yytext );
`               trataToken( BACKQUOTE, yytext );
\>              trataToken( MAYOR, yytext );
\<              trataToken( MENOR, yytext );
\>\>            trataToken( MAYORMAYOR, yytext );
2\>             trataToken( DOSMAYOR, yytext );
2\>\>           trataToken( DOSMAYORMAYOR, yytext );
@[0-9]+         trataToken( ARROBA, yytext );
\%              trataToken( PORCIENTO, yytext );
[ \t]+          trataToken( SPACE, yytext );
.               trataChar( yytext[0] );

%%

/* $Id: shell.l 1399 2007-12-20 09:45:07Z pedroe $ */
