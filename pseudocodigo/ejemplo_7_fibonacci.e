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

  Programa que calcula N términos de la serie de fibonacci
#

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
