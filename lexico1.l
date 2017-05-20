%{
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include "macros.h"
#include "ejemplo1.h"
#include "ejemplo1.tab.h"
extern char *progname;
extern int lineno;
%}
/* definiciones regulares */
numero        [0-9]
letra         [a-zA-Z]
identificador {letra}(({letra}|{numero}|_({letra}|{numero}))*)?
espacio 		[\t\n]

%x COMENTARIO

%%
[ \t] { ; } /* saltar los espacios y los tabuladores */

"#" {BEGIN COMENTARIO;}
<COMENTARIO>"#"	{BEGIN 0;} /* Vuelve al estado normal */
<COMENTARIO>(.|{espacio})	{;}

"@".*\n	{;} /* En los comentarios se ignoran todos los caracteres */

\'((\\\')?.*)*\' {
            char *s;
            s = malloc(sizeof(char)*(strlen(yytext)+1));
            sscanf(yytext,"%s",s);
            /* Instala la cadena en la tabla de símbolos */
            Installval v;
            v.str = yytext;
            yylval.sym=install("",STRING,INDEFINIDA,v);
            return STRING;}

{numero}+\.?|{numero}*\.{numero}+|{numero}*(\.{numero}*)?[eE]{numero}+ {
            double d;  
            sscanf(yytext,"%lf",&d);
            /* Instala el numero en la tabla de simbolos */
            Installval v;
            v.num = d;
				    yylval.sym=install("",NUMBER,INDEFINIDA,v); 
				    return NUMBER;}

{identificador} { Symbol *s;
          int i;
					for(i = 0; yytext[i]; i++) {
						yytext[i] = tolower(yytext[i]);
					}
          Installval v;
          v.str = 0;
          if ((s=lookup(yytext)) == 0)
                  s = install (yytext, INDEFINIDA,INDEFINIDA, v);
          yylval.sym = s;
          return s->tipo == INDEFINIDA ? VAR : s->tipo;}

">="	{return MAYOR_IGUAL;}
"<="	{return MENOR_IGUAL;}
"=="	{return IGUAL;}
"<>"	{return DISTINTO;}
">"		{return MAYOR_QUE;}
"<"		{return MENOR_QUE;}
"_no"	{return NEGACION;}
"_o"	{return O_LOGICO;}
"_y"	{return Y_LOGICO;}
":="    {return ASIGNACION;}
"_mod"	{return MODULO;}
"_div"	{return DIVIDIR_INT;}
"**"	{return POTENCIA;}
 	 
\n      {lineno++; } /* no se devuelve nada, continúa el analisis léxico */
.       {return yytext[0];}
