
Modo de obtención del intérprete
================================
Para poder compilar con éxito el intérprete son necesarios los siguientes
ficheros:

 - `code.c`: incluye el código para todas las funciones que ejecutan funcionalidad
   del pseudocódigo.

 - `init.c`: inicializa las constantes, palabras clave y las funciones de
   disponibles en el pseudocódigo.

 - `ipe.h`: cabecera para todo el programa, define las estructuras usadas.

 - `ipe.y`: fichero de *Bison* con las reglas de la gramática.

 - `lexico.l`: fichero de *Flex* con las reglas del léxico.

 - `math.c`: fichero que define las funciones matemáticas disponibles.

 - `symbol.c`: fichero con funciones que manipulan la tabla de símbolos.

El `Makefile` usado se ha creado a partir de algunas modificaciones sobre el
fichero proporcionado al principio. Los cambios necesarios en cuestión, han sido:

 - Cambiar la variable `FUENTE` a `ipe`, para cambiar el nombre de los ficheros
   principales.

 - Borar el número de versión en `VERSION` para que no necesiten sufijo numérico
   los ficheros.

El proceso de compilación coniste en generar el archivo de *Bison* y compilarlos,
generar el archivo de *Flex* y compilarlo. Luego se compilan el resto de archivos
`.c` y finalmente se unen todos los archivos objeto para crear el ejecutable
del intérprete.


Modo de ejecución del intérprete
================================
El intérprete dispone de dos modos de ejecución. Se ha añadido funcionalidad
para que el comportamiento sea ligeramente distinto en cada modo, de esta
forma se adapta mejor a las necesidades de cada modo.

Modo interactivo
----------------
En este modo, el intérprete se encarga de evaluar cada una de las órdenes
introducidas por el usuario de forma inmediata. En este modo las salidas por
pantalla usandos las funciones de escritura, se prefijan con `--->`. Y las
llamadas a `leervariable` y `leercadena` piden los datos con los prefijos
`Número-->` y `Cadena-->` respectivamente.

*Ejemplo de salida del modo interactivo*:

```
dato:=3;
escribir(dato);
	 ---> 3
escribir_cadena('hola ' || 'mundo');
	 ---> hola mundo
```

Modo *batch*
------------
En este modo el intérprete toma un fichero con el pseudocódigo y lo ejecuta.
Cuado este modo está activo, se quitan las pistas visuales para el usuario en las
funciones de escritura y lectura.

*Ejemplo llamada a modo batch*:

```
$ ./ipe.exe fichero_fuente.e
```

Ejemplos
========

Ejemplos previos
----------------
A continuación se listan los ejemplos proporcionados por el profesor:

- `ejemplo_1_saluda.e`: pide el nombre al usuario y lo saluda con un mensaje.

- `ejemplo_2_factorial.e`: pide un número al usuario y calcula el factorial.

- `ejemplo_3_mcd.e`: pide dos números al usuario y calcula su máximo común
   divisor.

- `ejemplo_4_menu_inicial.e`: muestra un menú de prueba al usuario.

- `ejemplo_5_menu_completo.e`: muestra un menú que permite obtener el factorial
   y el máximo común divisor.

- `ejemplo_6_Intercambiar_tipos.e`: fichero cuyo objetivo es comprobar si las
   variables tienen carácter dinámico.

Ejemplos añadidos
-----------------
Se han creado tres nuevos ejemplos, siendo el últimos ellos 
`ejemplo_9_fib-prim.e` un agregado de los anteriores junto con un menú.

### Ejemplo 1
En este ejemplo se pide un número al usuario, y se calculan tantos términos
de la serie según la magnitud del número leído. El código se encuentra en el
fichero `ejemplo_7_fibonacci.e`.

```
_borrar;
_lugar(10,10);
escribir_cadena('Serie de fibonacci\n');
_lugar(11,10);
escribir_cadena('Introduce el número de términos--> ');
leer(N);

a := 0;
b := 1;

_borrar;
escribir_cadena('Primeros ');
escribir(N);
escribir_cadena(' términos de la serie:\n');

para i desde 1 hasta N paso 1
hacer
  aux := a;
  a := b;
  b := aux + b;
  escribir_cadena('\t');
  escribir(aux);
  escribir_cadena('\n');
fin_para;
```

### Ejemplo 2
En este ejemplo se pide al usuario un número, y se comprueba si es primo o no.
El código se encuentra en el fichero `ejemplo_8_primalidad.e`.

```
_borrar;
_lugar(10,10);
escribir_cadena('Test de primalidad\n');
escribir_cadena('Introduce un número--> ');
leer(N);

primo := 1;

para i desde 2 hasta N-1 paso 1
hacer
  si ((N _mod i) = 0) entonces
    primo := 0;
  fin_si;
fin_para;

_borrar;
_lugar(10,10);
si (primo = 1) entonces
  escribir(N);
  escribir_cadena(' es primo\n');
si_no
  escribir(N);
  escribir_cadena(' no es primo\n');
fin_si;
```

Conclusiones
============

Conclusión sobre el trabajo
---------------------------
Realizar este trabajo ha servido tanto para afianzar los contenidos vistos en
las clases de teoría, como para pero ver alguna de las aplicaciones prácticas
de los contenidos vistos.

También se comprende algo más el funcionamiento de los intérpretes de lenguajes
y compiladores. Aunque no se llega a profundizar demasiado, sirve para
respetar y admirar la gran complejidad de las herramientas que proporcionan los
lenguajes de programación que usamos a diario.

Por otro lado, este trabajo también nos ha permitido afianzar las habilidades
prácticas para implementar programas que analicen datos estructurados. Por
ejemplo ficheros de configuración o formatos para transmisión de datos (ej.json).
O sentar las bases sobre las que seguir aprendiendo para poder participar en el
desarrollo de lenguajes de programación actuales o futuros.

Puntos débiles y fuertes del intérprete
---------------------------------------
Los principales puntos fuertes de este intérprete son su capacidad para leer
ficheros e interactuar con el usuario. Además el hecho de que acepte un
pseudocódigo en español puede facilitar el entendimiento a algunas personas
que tienen dificultad con el inglés.

También se puede resaltar el hecho de que al ser un intérprete bastante simple,
es adecuado para labores educativas. Ya que permite pronfundizar en los distintos
aspectos de los intérpretes sin llegar a aumentar la complejidad de forma
excesiva.

Como puntos débiles se pueden destacar su número limitado de tipos, así como
una incapacidad de convertir entre ellos. Además, la incapacidad de definir
funciones propias limita seriamente la capacidad del usuario para crear
programas complejos y que cumplan los requisitos de reutilización y
claridad en la estructua del código.

Los otros puntos débiles del intérprete se pueden encontrar en la limitación
en la longitud de los programas, y el la mala gestión de la memoria que realiza.