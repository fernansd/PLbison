
Análisis léxico
===============
El fichero proporcionado a *Flex* para el análisis léxico es `lexico.l`.
A continuación se procede a realizar una análisis de las partes que componen
al fichero.

Definiciones
------------
En esta parte se definen tanto las expresiones regulares como los estados
que se van a usar. Se definen varias expresiones regulares que pueden resultar
útiles. Además también se define el estado `COMENTARIO` usado para poder
procesar los comentarios multilínea.

```{.c .numberLines startFrom="12"}
/* definiciones regulares */
numero        [0-9]
letra         [a-zA-Z]
identificador {letra}(({letra}|{numero}|_({letra}|{numero}))*)?
identificador_malo ({letra}|{numero}|_)+
espacio 		[\t\n]

%x COMENTARIO
```

Reglas
------
A partir de la línea 20 del fichero se encuentran todas las reglas usadas por
flex para realizar el análisis. En la primera parte se definen las reglas
para leer comentarios, cadenas y números. Mientras que en la segunda parte
se encuentran las reglas para reconocer operadores y los comandos especiales.

> *Nota:* Se ha omitido el código ejecutado por las reglas para reducir la
longitud del fragmento.

```{.c}
[ \t] { ; } /* saltar los espacios y los tabuladores */

"#" {BEGIN COMENTARIO;}
<COMENTARIO>"#"	{BEGIN 0;} /* Vuelve al estado normal */
<COMENTARIO>\n   {lineno++;}
<COMENTARIO>.   {;}

"@".*\n	{;} /* En los comentarios se ignoran todos los caracteres */

'([^'\n]|\\\')*' {...}

'''([^']|\\\')*''' {...}

{numero}+\.?|{numero}*\.{numero}+|{numero}*(\.{numero}*)?[eE]{numero}+ {...}

{identificador} {...}

"||"  {return CONCATENAR;}
">="	{return MAYOR_IGUAL;}
"<="	{return MENOR_IGUAL;}
"="	{return IGUAL;}
"<>"	{return DISTINTO;}
 ...
```

Análisis sintáctico
===================

Estructura del fichero
----------------------

El fichero utilizado por *Bison* para definir la gramática es `ipe.y`.
A continuación se va a proceder a explicar las partes. Se ha evitado incluir
código ya que se haría inncesariamente extenso el documento.

 - En la primera parte del fichero se pueden incluir declaraciones en C, en este
   caso son algunos *include* y *define*.

 - La segunda parte (líneas 26-42) de un fichero de *Bison* son las declaraciones, 
   en este parte se definen los token que se usarán así como las preferencias.

 - La tercera parte (líneas 45-147) contiene todas las reglas de la gramática
   que debe tener en cuenta *Bison*.

 - La cuarta parte del fichero está destinada a código en C que se desee
   incluir.


Modificaciones realizadas
-------------------------
Se han añadido a la parte de declaraciones los nuevos símbolos terminales y no
terminales necesarios.

```{.yacc}
%token <sym> NUMBER STRING VAR CONSTANTE FUNCION0_PREDEFINIDA FUNCION1_PREDEFINIDA FUNCION2_PREDEFINIDA INDEFINIDA
%token <sym> PRINT WHILE DO WHILE_END IF THEN ELSE IF_END READ REPEAT UNTIL FOR FROM STEP FOR_END
%token <sym> READ_STR PRINT_STR CLEAN POSITION
%type <inst> stmt asgn expr stmtlist cond mientras si repetir para variable end 
%right ASIGNACION
%left O_LOGICO
%left Y_LOGICO
%left MAYOR_QUE MENOR_QUE MENOR_IGUAL MAYOR_IGUAL DISTINTO IGUAL
%left '+' '-' CONCATENAR
%left '*' '/' DIVIDIR_INT MODULO
%left UNARIO NEGACION
%right POTENCIA   
```

Se han añadio las siguientes reglas de producción:

```{.yacc}
| READ_STR '(' VAR ')' {code2(leercadena,(Inst)$3);}
| CLEAN         {code(borrarpantalla);}
| POSITION '(' NUMBER ',' NUMBER ')' {code3(posicion,(Inst)$3,(Inst)$5);}
...
| repetir stmtlist end UNTIL cond end
...
| para variable FROM expr end UNTIL expr end STEP expr end DO stmtlist end FOR_END
...
variable: VAR {code((Inst)$1); $$=progp;}
		;
...
repetir:     REPEAT     {$$= code3(repeatcode,STOP,STOP);}
        ;

para:        FOR        {$$= code3(forcode,STOP,STOP); code2(STOP,STOP);}
...
```

Además se han modificado algunas reglas de producción ya existentes. Entre ellas
se ha excluido la que permitía bloques delimitados por llaves. También se han
modificado las estructuras de control para incluir *token* que indiquen el fin,
por ejemplo `fin_si` para el condicional.

En la función `main` se han realizado algunas modificiones para permitir que
exista una variable global que indique al resto de funciones si se está
ejecutando el intérprete en modo interactivo o *batch*.


**Bucle repetir**

Para el *bucle repetir*, sólo es necesario guardar la dirección de la condición
de parada del bucle y la dirección de la siguiente instrucción. Para acceder
al cuerpo del bucle, se accede a la posición siguiente a donde se guarda la
dirección de la siguiente instrucción.

```{.yacc}
| repetir stmtlist end UNTIL cond end
          {
            ($1)[1]=(Inst)$5; /* condición de repetir */
            ($1)[2]=(Inst)$6; /* siguiente instruccion */
          }
...
repetir:     REPEAT     {$$= code3(repeatcode,STOP,STOP);}
```

*Estructuras de las intrucciones*:

```
	 STOP <1>->
	 STOP <2>->
	 cuerpo
<1>->cond
	 STOP
<2>->...
```


**Bucle para**

En este bucle, lo que se necesita guardar en las posiciones contiguas al código
del bucle, es el valor de parada, el paso, el cuerpo del bucle y el de la
siguiente instrucción. Se supone que a continuación estarán en memoria la
dirección de la variable usada para el bucle y la de la expresión que inicializa
la variable.

```{.yacc}
| para variable FROM expr end UNTIL expr end STEP expr end DO stmtlist end FOR_END
          {
            ($1)[1]=(Inst)$7;  /* valor final */
            ($1)[2]=(Inst)$10; /* paso entre cada iteracion */
            ($1)[3]=(Inst)$13; /* cuerpo del bucle */
            ($1)[4]=(Inst)$14; /* end */
          }
...
para:        FOR        {$$= code3(forcode,STOP,STOP); code2(STOP,STOP);}
```

*Estructuras de las intrucciones*:

```
	 STOP <1>->
	 STOP <2>->
	 STOP <3>->
	 STOP <4>->
	 VAR
	 expr1
	 STOP
<1>->expr2
	 STOP
<2>->expr3
	 STOP
<3>->cuerpo
	 STOP
<4>->...
```

Funciones auxiliares
====================
En esta sección se describen las funciones auxiliares que se han tenido que
crear para ofrecer la funcionalidad que actualmente tiene el intérprete. Todas
estas funciones se encuentran en el fichero `code.c`.

 - `execute`(l.83): se ha añadido control de errores para evitar errores en
   programas con una mala sintaxis.

 - `dividir_int`(l.151): función encargada de hacer la división entera.

 - `escribir`(l.168): modificada para poder imprimir cadenas.

 - `borrarpantalla`(l.208): función asociada al comando `_borrar`.

 - `posicion`(l.213): función asociada al comando `_lugar`.

 - `concatenar`(l.342): función asociada al operador `||`.

 - `leercadena`(l.399): función asociada al comando `leer_cadena`.

 - `evaluarcadena`(l.427): función encargar de sustituir los caracteres
    escapados en una cadena.
 - `mayor_que`(l.456): adaptada para aceptar cadenas.

 - `menor_que`(l.485): adaptada para aceptar cadenas.

 - `igual`(l.514): adaptada para aceptar cadenas.

 - `mayor_igual`(l.542): adaptada para aceptar cadenas.

 - `menor_igual`(l.571): adaptada para aceptar cadenas.

 - `distinto`(l.599): adaptada para aceptar cadenas.

 - `repeatcode`(l.726): código del bucle *repetir*.

 - `forcode`(l.747): código del bucle *para*.

En el fichero `symbol.c` en la línea 47 comienza la función `install`, la cual
se ha modificado para poder instalar cadenas.