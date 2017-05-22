#
  Asignatura:    Procesadores de Lenguajes

  Titulación:    Ingeniería Informática
  Especialidad:  Computación
  Curso:         Tercero
  Cuatrimestre:  Segundo

  Departamento:  Informática y Análisis Numérico
  Centro:        Escuela Politécnica Superior de Córdoba
  Universidad de Córdoba
 
  Curso académico: 2016 - 2017

  Programa que comprueba si un número es primo
#

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