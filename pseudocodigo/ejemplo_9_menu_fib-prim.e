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

  Fichero de ejemplo nº 5 para el intérprete de pseudocódigo en español: ipe.exe
#

@ Bienvenida

_borrar;

_lugar(10,10);

escribir_cadena('Introduce tu nombre --> ');

leer_cadena(nombre);

_borrar;
_lugar(10,10);

escribir_cadena(' Bienvenido/a << ');

escribir_cadena(nombre);

escribir_cadena(' >> al intérprete de pseudocódigo en español:\'ipe.exe\'.');

_lugar(40,10);
escribir_cadena('Pulsa una tecla para continuar');
leer_cadena( pausa);


repetir 

 @ Opciones disponibles

 _borrar;

 _lugar(10,10);
 escribir_cadena(' Términos de fibonacci -->  1 ');

 _lugar(11,10);
 escribir_cadena(' Test de primalidad ---->   2 ');

 _lugar(12,10);
 escribir_cadena(' Finalizar ---------------> 0 ');

 _lugar(15,10);
 escribir_cadena(' Elige una opcion--> ');

 leer(opcion);

 _borrar;

 si (opcion  = 0)        @ Fin del programa
    entonces  
        _lugar(10,10);
        escribir_cadena(nombre);
        escribir_cadena(': gracias por usar el intérprete ipe.exe ');

   si_no
       @ Términos de fibonacci
	si (opcion = 1)
   	    entonces
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

		    
   	@ Máximo común divisor
	si_no 
		si (opcion = 2)
			entonces
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

			@ Resto de opciones
 			si_no  
				_lugar(15,10);
			    	escribir_cadena(' Opcion incorrecta ');

 			fin_si;   
  	fin_si;                 

  fin_si;                          

 _lugar(40,10); 
 escribir_cadena('\n Pulse una tecla para continuar --> ');
 leer_cadena(pausa);
 
hasta (opcion = 0);             

@ Despedida final

_borrar;
_lugar(10,10);
escribir_cadena('El programa ha concluido\n');
