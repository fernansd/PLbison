#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

#include "ipe.h"
#include "ipe.tab.h"

#include "macros.h"

#define NSTACK 256              /* Dimension maxima de la pila */
static Datum stack[NSTACK];     /* La pila */
static Datum *stackp;           /* siguiente lugar libre en la pila */

#define NPROG 2000 
Inst prog[NPROG];  /* La maquina */
Inst *progp;       /* Siguiente lugar libre para la generacion de codigo */

Inst *pc; /* Contador de programa durante la ejecucion */

int modo_interprete; /* Indica si la ejecución es en modo intérprete (1) */
                     /* o en modo batch (0) */

void initcode() /* inicializacion para la generacion de codigo */
{
 stackp = stack;
 progp = prog;
}

void push(Datum d) /* meter d en la pila */
{
 
/* Comprobar que hay espacio en la pila para el nuevo valor o variable */
 
 if (stackp >= &stack[NSTACK])
     execerror (" Desborde superior de la pila ", (char *) 0);
 
 *stackp++ =d; /* Apilar la variable o el numero y */
               /* desplazar el puntero actual de la pila */
}


Datum pop() /* sacar y devolver de la pila el elemento de la cima */
{
 
/* Comprobar que no se intenta leer fuera de la pila */ 
/* En teoria no ocurrira nunca */
 
 if (stackp <= stack)
     execerror (" Desborde inferior de la pila ", (char *) 0);
 
 --stackp;          /* Volver hacia atras una posicion en la pila */
 return(*stackp);   /* Devolver variable o numero */
}

void pop2() /* sacar y  NO devolver el elemento de la cima de la pila */
{
 
/* Comprobar que no se intenta leer fuera de la pila */ 
/* En teoria no ocurrira nunca */
 
 if (stackp <= stack)
     execerror (" Desborde inferior de la pila ", (char *) 0);
 
 --stackp;          /* Volver hacia atras una posicion en la pila */
}

Inst *code(Inst f) /* Instalar una instruccion u operando */
{
 Inst *oprogp = progp;   /* Puntero auxiliar */
 
/* Comprobar que hay espacio en el vector de instrucciones */ 

 if (progp >= &prog[NPROG])
     execerror (" Programa demasiado grande", (char *) 0);
 
 *progp=f;        /* Asignar la instruccion o el puntero a la estructura */
 progp++;         /* Desplazar una posicion hacia adelante */
 return (oprogp);
}


void execute(Inst *p)  /* Ejecucion con la maquina */
{
 
/* El contador de programa pc se inicializa con la primera instruccion a */ 
/* ejecutar */
 
  for (pc=p; *pc != STOP; ) {
    (*(*pc++))();              /* Ejecucion de la instruccion y desplazar */
                               /* el contador de programa pc */
    if (pc == NULL) {
      fprintf(stderr, " Error. Se intentó ejecutar una instrucción inválida, revise"
                      " la sintaxis\n");
      break;
    }
  }
}
/****************************************************************************/
/****************************************************************************/

void assign() /* asignar el valor superior al siguiente valor */
{
 Datum d1,d2;
 d1=pop();    /* Obtener variable */
 d2=pop();    /* Obtener valor    */
 
 if (d1.sym->tipo != VAR && d1.sym->tipo != INDEFINIDA)
   execerror(" asignacion a un elemento que no es una variable ", 
	     d1.sym->nombre);
 if (d2.subtipo == STRING) {
  d1.sym->u.str=d2.str;  /* Asignar cadena */
 } else {
  d1.sym->u.val=d2.val;   /* Asignar numero */
 }
  d1.sym->tipo=VAR;
  d1.sym->subtipo=d2.subtipo;
  push(d2);               /* Apilar variable */
}

void constpush()  /* meter una constante en la pila */
{
 Datum d;
 Symbol *s = ((Symbol *)*pc++);
 if (s->tipo==STRING) {
  d.str = s->u.str;
  d.subtipo = STRING;
 } else { /* tipo NUMBER */
  d.val= s->u.val;
  d.subtipo = NUMBER;
 }
 push(d);
}

void dividir() /* dividir los dos valores superiores de la pila */
{
 Datum d1,d2;
 
 d2=pop();      /* Obtener el primer numero  */
 d1=pop();      /* Obtener el segundo numero */
 
/* Comprobar si hay division por 0 */ 
 
 if (d2.val == 0.0)
     execerror (" Division por cero ", (char *) 0);
 
 d1.val = d1.val / d2.val;    /* Dividir             */
 push(d1);                    /* Apilar el resultado */
}

void dividir_int() /* division entera de los dos valores superiores de la pila */
{
 Datum d1,d2;
 
 d2=pop();      /* Obtener el primer numero  */
 d1=pop();      /* Obtener el segundo numero */
 
/* Comprobar si hay division por 0 */ 
 int n1 = (int)d1.val;
 int n2 = (int)d2.val;
 if (n2 == 0)
     execerror (" Division por cero ", (char *) 0);
 
 d1.val = n1 / n2;        /* Dividir             */
 push(d1);                /* Apilar el resultado */
}

void escribir() /* sacar de la pila el valor superior y escribirlo */
{
 Datum d;
 
 d=pop();  /* Obtener numero */

 if (d.subtipo == STRING) {
  if (modo_interprete)
    printf("\t ---> %s\n", d.str);
  else
    printf("%s", d.str);
 } else if (d.subtipo == NUMBER) {
  if (modo_interprete)
    printf("\t ---> %.8g\n",d.val);
  else
    printf("%.8g",d.val);
 } else {
  execerror(" Error. Tipo a escribir desconocido", (char *) 0);
 }
}

void eval() /* evaluar una variable en la pila */
{
 Datum d;
 
 d=pop();  /* Obtener variable de la pila */
 
/* Si la variable no esta definida */ 
 if (d.sym->tipo == INDEFINIDA) 
     execerror (" Variable no definida ", d.sym->nombre);
 if (d.sym->subtipo == STRING) {
    d.str = d.sym->u.str;
    d.subtipo = STRING;
 } else { /* subtipo NUMBER */
    d.val = d.sym->u.val;
    d.subtipo = NUMBER;
 }
 push(d);             /* Apilar valor */
}

void borrarpantalla()
{
  printf("\33[2J") ;
}

void posicion()
{
  Symbol* s1 = (Symbol*)(*pc);
  Symbol* s2 = (Symbol*)(*(pc+1));
  if (s1->tipo != NUMBER || s2->tipo != NUMBER) {
    execerror(" Ambos argumentos deben ser numéricos", (char*)0);
  }

  printf("\033[%d;%dH", (int)s1->u.val, (int)s2->u.val);
  pc = pc+2;
}

void funcion0() /* evaluar una funcion predefinida sin parametros */
{
 Datum d;
 
 d.val= (*(double (*)())(*pc++))();
 push(d);
}

void funcion1() /* evaluar una funcion predefinida con un parametro */
{
 Datum d;
 
 d=pop();  /* Obtener parametro para la funcion */

 d.val= (*(double (*)())(*pc++))(d.val);
 push(d);
}

void funcion2() /* evaluar una funcion predefinida con dos parametros */
{
 Datum d1,d2;
 
 d2=pop();  /* Obtener parametro para la funcion */
 d1=pop();  /* Obtener parametro para la funcion */

 d1.val= (*(double (*)())(*pc++))(d1.val,d2.val);
 push(d1);
}

/* resto de la division entera del segundo valor de la pila */
/* por el valor de la cima */
void modulo() 
{
 Datum d1,d2;
 
 d2=pop();      /* Obtener el divisor */
 d1=pop();      /* Obtener el dividendo */
 
/* Comprobar si hay division por 0 */ 
 
 if (d2.val == 0.0)
     execerror (" Division por cero ", (char *) 0);
 
 d1.val = (int) d1.val % (int)  d2.val;  /* Resto */
 push(d1);                               /* Apilar el resultado */
}

void multiplicar() /* multiplicar los dos valores superiores de la pila */
{
 Datum d1,d2;
 
 d2=pop();                   /* Obtener el primer numero  */
 d1=pop();                   /* Obtener el segundo numero */
 d1.val = d1.val * d2.val;   /* Multiplicar               */
 push(d1);                   /* Apilar el resultado       */
}

void negativo() /* negacion del valor superior de la pila */
{
 Datum d1;
 
 d1=pop();              /* Obtener numero   */
 d1.val = - d1.val;     /* Aplicar menos    */
 push(d1);              /* Apilar resultado */
}

/* Esta funcion se puede omitir   */
void positivo() /* tomar el valor positivo del elemento superior de la pila */
{
 Datum d1;
 
 d1=pop();              /* Obtener numero   */
 /* d1.val = + d1.val;*/     /* Aplicar mas    */
 push(d1);              /* Apilar resultado */
}

void potencia()  /* exponenciacion de los valores superiores de la pila */
{
 Datum d1,d2;
 
 d2=pop();                      /* Obtener exponente   */
 d1=pop();                      /* Obtener base        */
 
 if ( (d1.val>=0) || ((int)d2.val == d2.val) )
  {
   d1.val = pow(d1.val,d2.val);   /* Elevar a potencia   */
   push(d1);                      /* Apilar el resultado */
  }
 else 
  {
   char digitos[20];
   sprintf(digitos,"%lf",d1.val);
   execerror(" radicando negativo ", digitos);
  }

}

void restar()   /* restar los dos valores superiores de la pila */
{
 Datum d1,d2;
 
 d2=pop();                   /* Obtener el primer numero  */
 d1=pop();                   /* Obtener el segundo numero */
 d1.val = d1.val - d2.val;   /* Restar                    */
 push(d1);                   /* Apilar el resultado       */
}

void sumar()   /* sumar los dos valores superiores de la pila */
{
 Datum d1,d2;
 
 d2=pop();                   /* Obtener el primer numero  */
 d1=pop();                   /* Obtener el segundo numero */
 d1.val = d1.val + d2.val;   /* Sumar                     */
 push(d1);                   /* Apilar el resultado       */
}

void concatenar() {
  Datum d1, d2;
  d2 = pop();
  if (d2.subtipo != STRING) {
    execerror(" segundo operando no es una cadena", (char*) 0);
    printf("\t\t tipo: %d\n",d2.subtipo);
  }
  d1 = pop();
  if (d1.subtipo != STRING) {
    execerror(" primer operando no es una cadena", (char*) 0);
    printf("\t\t tipo: %d\n",d1.subtipo);
  }

  Datum d;
  int len = strlen(d1.str) + strlen(d2.str);
  d.str = malloc(sizeof(char)*(len+1));
  strcpy(d.str, d1.str);
  strcat(d.str, d2.str);
  d.subtipo = STRING;
  push(d);
}

void varpush()  /* meter una variable en la pila */
{
 Datum d;

 d.sym=(Symbol *)(*pc++);
 push(d);
}
/****************************************************************************/
/****************************************************************************/

void leervariable() /* Leer una variable numerica por teclado */
{
 Symbol *variable;
 char c;

 variable = (Symbol *)(*pc); 

 /* Se comprueba si el identificador es una variable */ 
  if ((variable->tipo == INDEFINIDA) || (variable->tipo == VAR))
  {
    if (modo_interprete)
      printf("Número--> ");

    while((c=getchar())=='\n') ;
    ungetc(c,stdin);
    scanf("%lf",&variable->u.val);
    variable->tipo=VAR;
    variable->subtipo=NUMBER;
    pc++;

  } else {
     execerror("No es una variable",variable->nombre);
  }
}

void leercadena()
{
  Symbol *variable;
  char s[128]; /* Buffer de lectura */

  variable = (Symbol *)(*pc);

  if ((variable->tipo == INDEFINIDA) || (variable->tipo == VAR))
  {
    if (modo_interprete)
      printf("Cadena--> ");

    while((s[0]=getchar())=='\n') ;
    fgets(&s[1],127,stdin);
    int len = strlen(s);
    s[len-1] = '\0'; /* Elimina \n del final */

    variable->u.str = malloc(sizeof(char)*(len+1));
    strcpy(variable->u.str, s);
    evaluarcadena(variable->u.str);
    variable->tipo=VAR;
    variable->subtipo=STRING;
    pc++;
  } else {
    execerror("No es una variable",variable->nombre);
  }
}

void evaluarcadena(char *s)
{
  int i = 0; /* Variable para recorrer la cadena original */
  int pos = 0; /* Variable para insertar la cadena evaluada */
  while (s[i] != '\0') {
    if (s[i] == '\\') { /* Compramos si es un carácter escapado */
      i++;              /* y sustituimos si es necesario        */
      if (s[i] == 't') {
        s[pos] = '\t';
      } else if (s[i] == 'n') {
        s[pos] = '\n';
      } else if (s[i] == '\'') {
        s[pos] = '\'';
      } else if (s[i] == '\\') {
        s[pos] = '\\';
      } else { /* Para caracteres que no corresponden a uno escapado */
        s[pos++] = '\\';
        s[pos] = s[i];
      }
    } else { /* Para caracteres normales */
      s[pos] = s[i];
    }
    i++;
    pos++;
  }
  s[pos] = '\0';
}


void mayor_que()
{
 Datum d1,d2;
 
 d2=pop();   /* Obtener el primer numero  */
 d1=pop();   /* Obtener el segundo numero */

 if (d1.subtipo == STRING && d2.subtipo == STRING) {
  if (strcmp(d1.str, d2.str) == 1)
    d1.val = 1;
  else
    d1.val = 0;

 } else if (d1.subtipo == NUMBER && d2.subtipo == NUMBER) {
   if (d1.val > d2.val)
     d1.val = 1;
   else
     d1.val = 0;

 } else {
    execerror(" No concuerdan los tipos de los operandos", (char*)0);
 }

 d1.subtipo = NUMBER; /* A los valores booleanos se les trata como números */
 
 push(d1);  /* Apilar resultado */
}


void menor_que()
{
 Datum d1,d2;
 
 d2=pop();    /* Obtener el primer numero  */
 d1=pop();    /* Obtener el segundo numero */

 if (d1.subtipo == STRING && d2.subtipo == STRING) {
  if (strcmp(d1.str, d2.str) == -1)
    d1.val = 1;
  else
    d1.val = 0;

 } else if (d1.subtipo == NUMBER && d2.subtipo == NUMBER) {
   if (d1.val < d2.val)
     d1.val = 1;
   else
     d1.val = 0;

 } else {
    execerror(" No concuerdan los tipos de los operandos", (char*)0);
 }

 d1.subtipo = NUMBER; /* A los valores booleanos se les trata como números */
 
 push(d1);    /* Apilar el resultado */
}


void igual()
{
 Datum d1,d2;
 
 d2=pop();    /* Obtener el primer numero  */
 d1=pop();    /* Obtener el segundo numero */
 
 if (d1.subtipo == STRING && d2.subtipo == STRING) {
  if (strcmp(d1.str, d2.str) == 0)
    d1.val = 1;
  else
    d1.val = 0;

 } else if (d1.subtipo == NUMBER && d2.subtipo == NUMBER) {
   if (d1.val == d2.val)
     d1.val = 1;
   else
     d1.val = 0;

 } else {
    execerror(" No concuerdan los tipos de los operandos", (char*)0);
 }

 d1.subtipo = NUMBER; /* A los valores booleanos se les trata como números */
 
 push(d1);    /* Apilar resultado */
}

void mayor_igual()
{
 Datum d1,d2;
 
 d2=pop();    /* Obtener el primer numero  */
 d1=pop();    /* Obtener el segundo numero */
 
 if (d1.subtipo == STRING && d2.subtipo == STRING) {
  if (strcmp(d1.str, d2.str) >= 0)
    d1.val = 1;
  else
    d1.val = 0;

 } else if (d1.subtipo == NUMBER && d2.subtipo == NUMBER) {
   if (d1.val >= d2.val)
     d1.val = 1;
   else
     d1.val = 0;

 } else {
    execerror(" No concuerdan los tipos de los operandos", (char*)0);
 }

 d1.subtipo = NUMBER; /* A los valores booleanos se les trata como números */
 
 push(d1);    /* Apilar resultado */
}


void menor_igual()
{
 Datum d1,d2;
 
 d2=pop();     /* Obtener el primer numero  */
 d1=pop();     /* Obtener el segundo numero */
 
 if (d1.subtipo == STRING && d2.subtipo == STRING) {
  if (strcmp(d1.str, d2.str) <= 0)
    d1.val = 1;
  else
    d1.val = 0;

 } else if (d1.subtipo == NUMBER && d2.subtipo == NUMBER) {
   if (d1.val <= d2.val)
     d1.val = 1;
   else
     d1.val = 0;

 } else {
    execerror(" No concuerdan los tipos de los operandos", (char*)0);
 }

 d1.subtipo = NUMBER; /* A los valores booleanos se les trata como números */
 
 push(d1);     /* Apilar resultado */
}

void distinto()
{
 Datum d1,d2;
 
 d2=pop();    /* Obtener el primer numero  */
 d1=pop();    /* Obtener el segundo numero */
 
 if (d1.subtipo == STRING && d2.subtipo == STRING) {
  if (strcmp(d1.str, d2.str) != 0)
    d1.val = 1;
  else
    d1.val = 0;

 } else if (d1.subtipo == NUMBER && d2.subtipo == NUMBER) {
   if (d1.val != d2.val)
     d1.val = 1;
   else
     d1.val = 0;

 } else {
    execerror(" No concuerdan los tipos de los operandos", (char*)0);
 }

 d1.subtipo = NUMBER; /* A los valores booleanos se les trata como números */
 
 push(d1);    /* Apilar resultado */
}


void y_logico()
{
 Datum d1,d2;
 
 d2=pop();    /* Obtener el primer numero  */
 d1=pop();    /* Obtener el segundo numero */
 
 if (d1.val==1 && d2.val==1)
   d1.val= 1;
 else 
   d1.val=0;
 
 push(d1);    /* Apilar el resultado */
}


void o_logico()
{
 Datum d1,d2;
 
 d2=pop();    /* Obtener el primer numero  */
 d1=pop();    /* Obtener el segundo numero */
 
 if (d1.val==1 || d2.val==1)
   d1.val= 1;
 else
   d1.val=0;
 
 push(d1);    /* Apilar resultado */
}


void negacion()
{
 Datum d1;
 
 d1=pop();   /* Obtener numero */
 
 if (d1.val==0)
   d1.val= 1;
 else
   d1.val=0;
 
 push(d1);   /* Apilar resultado */
}


void whilecode()
{
 Datum d;
 Inst *savepc = pc;    /* Puntero auxiliar para guardar pc */

 execute(savepc+2);    /* Ejecutar codigo de la condicion */
 
 d=pop();    /* Obtener el resultado de la condicion de la pila */
 
 while(d.val)   /* Mientras se cumpla la condicion */
    {
     execute(*((Inst **)(savepc)));   /* Ejecutar codigo */
     execute(savepc+2);               /* Ejecutar condicion */
     d=pop();              /* Obtener el resultado de la condicion */
    }
 
/* Asignar a pc la posicion del vector de instrucciones que contiene */  
/* la siguiente instruccion a ejecutar */ 
 
 pc= *((Inst **)(savepc+1));  
}

void ifcode()
{
 Datum d;
 Inst *savepc = pc;   /* Puntero auxiliar para guardar pc */

 execute(savepc+3);   /* Ejecutar condicion */
 d=pop();             /* Obtener resultado de la condicion */
 
 
/* Si se cumple la condición ejecutar el cuerpo del if */
 if(d.val && *((Inst **)(savepc)))
   execute(*((Inst **)(savepc)));
 
/* Si no se cumple la condicion se comprueba si existe parte else   */
/* Esto se logra ya que la segunda posicion reservada contendria el */
/* puntero a la primera instruccion del cuerpo del else en caso de  */
/* existir, si no existe sera\A0 STOP, porque a la hora de generar    */
/* codigo se inicializa con STOP.                                   */

 else if  (*((Inst **)(savepc+1)))  /* parte else */
   execute(*((Inst **)(savepc+1)));
 

/* Asignar a pc la posicion del vector de instrucciones que contiene */  
/* la siguiente instruccion a ejecutar */ 
 
 pc= *((Inst **)(savepc+2));
}

void repeatcode()
{
 Datum d;
 Inst *savepc = pc;    /* Puntero auxiliar para guardar pc */

  /* Sólo se ejecuta el bucle si es válido en cuerpo */
  if (*((Inst **)(savepc+2))) {
    do{
       execute(savepc+2);               /* Ejecutar cuerpo */
       execute(*((Inst **)(savepc)));   /* Ejecutar condición */
       
       d=pop();              /* Obtener el resultado de la condicion */
    } while (!d.val);
  }
 
/* Asignar a pc la posicion del vector de instrucciones que contiene */  
/* la siguiente instruccion a ejecutar */ 
 
 pc= *((Inst **)(savepc+1));
}

void forcode()
{
  Inst *savepc = pc;
  Datum d;

  Symbol* variable = (Symbol*)*(savepc+4);
  /*printf("usada variable %s con valor %lf\n", variable->nombre, variable->u.val);*/
  variable->tipo = VAR;

  /* Inicializar variable del bucle */
  execute(savepc+5);
  d = pop();
  variable->u.val = d.val;
  /*printf("desde: %lf\n", variable->u.val);*/

  /* Valor de parada del bucle */
  execute((Inst*)*(savepc));
  d = pop();
  double until_var = d.val;
  /*printf("hasta: %lf\n", until_var);*/

  /* Valor del paso dado entre iteraciones */
  execute((Inst*)*(savepc+1));
  d = pop();
  double step = d.val;
  /*printf("paso: %lf\n", step);*/

  /* Sólo si es válido el bloque de instrucciones */
  if (*((Inst **)(savepc+2))) {
    while (variable->u.val <= until_var) {
      
      execute(*((Inst **)(savepc+2)));
      variable->u.val += step;
    }
  }

  /* Asignamos el pc a la posición de la siguiente instruccion */
  pc= *((Inst **)(savepc+3));
}
