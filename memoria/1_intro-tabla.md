
Introducción
============
Este trabajo consiste en la realización de un intérprete para un lenguaje de
pseudocódigo cuya especificación nos ha sido dada en el enunciado de la práctica.
Asimismo se han añadido ciertas mejoras que se han visto oportunas para obtener
tanto un lenguaje como un intérprete más robustos.

El intérprete admite dos modos de ejecución distintos, uno es el modo interactivo,
el cual permite introducir código que será evaluado de inmediato. El otro modo
consiste en proporcionar al intérprete un achivo del cual obtener el código a
ejecutar.

En cuanto a la estructura de este documento, primero procederemos a hablar sobre
el lenguaje, para después ir profundizando en los detalles de la implementación.
Por último, se mostrarán algunos ejemplos y se expondrán las conclusiones
obtenidas tras la realización de este trabajo.

Lenguage pseudocódigo
=====================

Componentes léxicos
-------------------
### Palabras reservadas
Las palabras reservadas del lenguaje no podrán ser utilizadas como identificadores,
se han reservado las siguientes palabras:

```
_mod, _div, _o, _y, _no, leer, leer_cadena, escribir, escribir_cadena
si, entonces, si_no, fin_si, mientras, hacer, fin_mientras, repetir, hasta
para, desde, hasta, paso, fin_para, _borrar, _lugar
```

Además de esta palabras se reservadas, el intérpre añade algunas variables
constantes, las cuales no puede ser modificadas:
```
PI, E, GAMMA, DEG, PHI
```

### Identificador
Los identificadores pueden estar compuestos de letras, dígitos o guiones bajos.
Sin embargo, se debe cumplir que: empiece por una letra, no acabe en guión bajo
y no haya dos guiones bajos seguidos.

*Ejemplo de identificador válido:*
```
iden, iden_1a, ident_a_1_3
```

*Ejemplo de identificadores no válidos:*
```
_iden, iden_1_, iden__3
```

### Número
Los números pueden ser de tipo entero, real o con notación científica.

*Ejemplo números válidos:*
```
38	5.289	3.1e10
```

*Ejemplo números no válidos:*
```
3.000.000	3,895	0x398
```
### Cadena
Las cadenas se delimitan por comillas simples y pueden contener cualquier
tipo de carácter a excepción de un salto de línea. Para permitir caracteres
especiales se han incluido los siguientes código de escape:

```
\t 		Tabulador
\n 		Salto de línea
\' 		Comilla simple
\\ 		Barra invertida
```

Además de las cadenas simples, también se ha añadido un nuevo tipo de cadena,
la cadena multilínea. Esta cadena permite la inclusión de saltos de línea. Su
sintaxis requiere de tres comillas simples delimitando cada extremo de la cadena.

*Ejemplo de cadenas válidas:*
```
'Cadena simple'
'\tCadena con caracteres escapados\n'
'''Cadena
multilínea
'''
```

### Operador de asignación
El operador de asignación es `:=`, similar al usado en el lenguaje *Pascal*.

### Operadores aritméticos
Los operadores aritméticos permitidos son:

- *Suma*: `+`, tanto unario como binario. Por ejemplo `+3` y `3+5`.

- *Resta*: `-`, tanto unaria como binaria. Por ejemplo `-7` y `2+6`.

- *Producto*: `*`, solo binario. Por ejemplo `3 * 7 = 21`.

- *División*: `/`, solo binaria. Por ejemplo `9 / 3 = 0`.

- *División entera*: `_div`, solo binaria. Por ejemplo `3 _div 2 = 1`.

- *Módulo*: `_mod`, solo binario. Por ejemplo `10 _mod 3 = 1`.

- *Potencia*: `**`, solo binaria. Por ejemplo `3 ** 2 = 9`.

### Operador alfanumérico
El operador alfanumérico `||` se trata de un operador binario encargado de
concatenar dos cadenas. Por ejemplo:

```
'hola ' || ' mundo' = 'hola mundo'
```

### Operadores relacionales de números y cadenas
Los operadores relacionales mostrados a continuación admiten tanto números como
cadenas. Existen los siguientes operadores:

- *Menor que*: `<`

- *Menor o igual que*: `<=`

- *Mayor que*: `>`

- *Igual que*: `=`

- *Distinto que*: `<>`

> Nota: cuando se usan para comparar cadenas se está realizando una comparación
léxico gráfica. En concreto la implementada por la función `strcmp` de la librería
estándar de C. *[Referencia](https://www.tutorialspoint.com/c_standard_library/c_function_strcmp.htm)*

### Operadores lógicos
Los operadores lógicos disponibles son:

- *Disyunción lógica*: `_o`

- *Conjunción lógica*: `_y`

- *Negación lógica*: `_no`

*Ejemplos de uso de operadores lógicos*:

```
(A >= 0) _y _no (control <> 'stop')
```

### Comentarios
Existe dos tipos de comentarios, los multilínea (delimitados por `#`) y los
de una línea (comienzan con `@`).

*Ejemplos de comentarios*:

```
@ Comentario de una línea

# Comentario
multilínea
#
```

### Punto y coma
El punto y coma `;` se usa como carácter delimitador, para indicar el fin
de una sentencia.

```
escribir_cadena('hola');
```


Sentencias
----------

### Asignación
La asignación se realiza desde una expresión númerica o alfanumérica hacia un
identificador. Los identificadores pueden cambiar el tipo asociado que tengan
previamente. No se puede asignar a las constantes declaradas por el intérprete.

*Ejemplo de asignación*:

```
cadena := 'hola';
numero := 38;
numero := cadena;  @ numero ahora vale 'hola'
```

### Lectura
Para leer entrada por teclado por parte del usuario se pueden usar `leer` y
`leer_cadena`. Para ambas se debe proporcionar un identificador para la
variable en la que se deba guardar lo leído.

Solo si se está ejecutando el intérprete en modo interactivo, se mostrará al
usuario una pista de lo que tiene que introducir. Por ejemplo `Cadena--> ` o 
`Número--> `.

```
leer(dato);  @ el usuario introduce 5
(dato = 5) = true
```

### Escritura
Para escribir por pantalla se puede usar `escribir` y `escribir_cadena`, pasando
como parámetro la expresión cuya evaluación se mostrará por pantalla. Para hacer
más robusto el intérprete y facilitar el uso, se permite que se usen de forma
intercambiable, sin importar el tipo de la variable. La razón por la que se ha
mantenido `escribir_cadena` es para evitar inconsistencias.

Solo si se está ejecutando en modo interactivo el intérprete, se mostrará el
resultado precedido de `--> ` para indicar que es la salida del comando.

```
escribir(3 + 5);  @ imprime 8
```

### Sentencias de control
A continuación se muestran todas las sentencias destinadas a controlar el flujo
de ejecución del programa.

#### Condicional
Se permite tanto con opción alternativa como sin ella.

> **si** *condición*

> **entonces** *sentencias*

> **si_no** *sentencias*

> **fin_si**

*Ejemplo*:

```
var := 3;
si (var = 3) entonces
	escribir('hola');
si_no
	escribir('adios');
fin_si;
@ escribe hola
```

#### Bucle *mientras*
Se ejecuta mientras se cumpla la condición. Evalúa la condición antes de realizar
cada iteración.

> **mientras** *condición* **hacer**

>> *sentencias*

> **fin_mientras**

*Ejemplo*:

```
dato := 1;
mientras (dato < 5) hacer
	dato := dato +1;
fin_mientras;
```

#### Bucle *repetir*
Este bucle se ejecuta hasta que se cumpla la condición. Evalúa la condición
después de realizar cada iteración.

> **repetir**

>> *sentencias*

> **hasta** *condición*

*Ejemplo*:

```
dato := 0;
repetir
	escribir('hola');
	dato := dato + 1;
hasta (dato = 3);
@ escribe hola 3 veces
```

#### Bucle *para*
Este bucle comienza asignando a una variable el valor inicial, e itera hasta
que alcanza el valor final. En cada iteración aumenta la variable en la
cantidad proporcionada como paso.

> **para** *identificador*

>> **desde** *expresión numérica 1*

>> **hasta** *expresión numérica 2*

>> **paso** *expresión numérica 3*

>> **hacer**

>>> *sentencias*

> **fin_para**

*Ejemplo*:

```
para i
	desde 1
	hasta 3
	paso 1
	hacer
		escribir('hola');
fin_para;
@ escribe hola 3 veces
```

### Comandos especiales
Se han incluido dos comando especiales para facilitar el desarrolo de programas.
El comando `_borrar` borra el contenido de la pantalla actual. Por otro lado
el comando `_lugar` permite colocar el cursor en la posición deseada de la 
pantalla.

```
_borrar;
_lugar(3,5);
@ borra la pantalla y sitúa el cursor en la posición (3,5)
```

Tabla de símbolos
=================
La tabla de símbolos usada para esta implementación es la que se proporcionaba.
Es decir, se ha implementado la tabla de símbolos como una lista simplemente
enlazada. Se ha estimado que el rendimiento y simplicidad proporcionados por
esta implementación son suficientes para el uso apropiado del intérprete.

A continuación se muestra la estructura usada para representar cada nodo de la
lista:

```{.c}
typedef struct Symbol { /* entrada en la tabla de simbolos */
	               char *nombre;
	               short tipo;  /* VAR, FUNCION, INDEFINIDA */
	               short subtipo; /* STRING, NUMBER */
	               struct {
		              double val;        /* si es NUMBER */
	               	  char *str; 		 /* si es STRING */
		              double (*ptr)();   /* si es FUNCION */
		            } u;
                       struct Symbol * siguiente;
	              } Symbol;
```