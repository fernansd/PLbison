/*

 Ejemplo 9
 
  Se ha ampliado el ejemplo 8 para permitir 
  - operadores relacionales y lógicos
  - sentencia condicional: if
  - sentencia iterativa: while
  - ejecutar las sentencias contenidas en un fichero indicado en la l’nea de comandos
    Por ejemplo:
       > ./ejemplo9.exe factorial.txt
*/

%{
#include <stdio.h>
#include <math.h>

#include "ejemplo1.h"

#include "macros.h"

#define code2(c1,c2)         code(c1); code(c2)
#define code3(c1,c2,c3)      code(c1); code(c2); code(c3)
%}

%union{             
       Symbol *sym;    /* puntero a la tabla de simbolos */
       Inst *inst;     /* instruccion de maquina */
}

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
%%

list :    /* nada: epsilon produccion */ 
        | list stmt  ';' {code(STOP); return 1;}
        | list error ';'   {yyerrok;} 
        ;

stmt :    /* nada: epsilon produccion */  {$$=progp;}
        | asgn          {code(pop2);}
        | PRINT '(' expr ')'    {code(escribir); $$ = $3;}
        | READ '(' VAR ')' {code2(leervariable,(Inst)$3);}
        | PRINT_STR '(' expr ')' {code(escribir); $$ = $3;}
        | READ_STR '(' VAR ')' {code2(leercadena,(Inst)$3);}
        | CLEAN         {code(borrarpantalla);}
        | POSITION '(' NUMBER ',' NUMBER ')' {code3(posicion,(Inst)$3,(Inst)$5);}
        | mientras cond DO stmtlist end WHILE_END
                  {
                   ($1)[1]=(Inst)$4; /* cuerpo del bucle */
                   ($1)[2]=(Inst)$5; /* siguiente instruccion al bucle */
                  }
        | si cond THEN stmtlist end IF_END/* proposicion if sin parte else */
                  {
                   ($1)[1]=(Inst)$4; /* cuerpo del if */
                   ($1)[3]=(Inst)$6; /* siguiente instruccion al if */
                  }
        | si cond THEN stmtlist ELSE stmtlist end IF_END/* proposicion if con parte else */
                  {
                   ($1)[1]=(Inst)$4; /* cuerpo del if */
                   ($1)[2]=(Inst)$7; /* cuerpo del else */
                   ($1)[3]=(Inst)$8; /* siguiente instruccion al if-else */
                  }
        | repetir stmtlist end UNTIL cond end
                  {
                    ($1)[1]=(Inst)$5; /* condición de repetir */
                    ($1)[2]=(Inst)$6; /* siguiente instruccion */
                  }
        | para variable FROM expr end UNTIL expr end STEP expr end DO stmtlist end FOR_END
                  {
                    ($1)[1]=(Inst)$7;  /* valor final */
                    ($1)[2]=(Inst)$10; /* paso entre cada iteracion */
                    ($1)[3]=(Inst)$13; /* cuerpo del bucle */
                    ($1)[4]=(Inst)$14; /* end */
                  }
        /* | '{' stmtlist '}'  {$$ = $2;} */
        ;

variable: VAR {code((Inst)$1); $$=progp;}


asgn :    VAR ASIGNACION expr { $$=$3; code3(varpush,(Inst)$1,assign);}
        | CONSTANTE ASIGNACION expr 
          {execerror(" NO se pueden asignar datos a constantes ",$1->nombre);}
	;

cond :    '(' expr ')' {code(STOP); $$ =$2;}
        ;

mientras:    WHILE      {$$= code3(whilecode,STOP,STOP);}
        ;

si:          IF         {$$= code(ifcode); code3(STOP,STOP,STOP);}
        ;

repetir:     REPEAT     {$$= code3(repeatcode,STOP,STOP);}
        ;

para:        FOR        {$$= code3(forcode,STOP,STOP); code2(STOP,STOP);}

end :    /* nada: produccion epsilon */  {code(STOP); $$ = progp;}
        ;

stmtlist:  /* nada: prodcuccion epsilon */ {$$=progp;}
        | stmtlist stmt ';'
        ;

expr :    NUMBER     		{$$=code2(constpush,(Inst)$1);}
        | STRING        {$$=code2(constpush,(Inst)$1);}
        | VAR        		{$$=code3(varpush,(Inst)$1,eval);} 
        | CONSTANTE      	{$$=code3(varpush,(Inst)$1,eval);}
        | asgn
        | FUNCION0_PREDEFINIDA '(' ')'      {code2(funcion0,(Inst)$1->u.ptr);}
        | FUNCION1_PREDEFINIDA '(' expr ')' {$$=$3;code2(funcion1,(Inst)$1->u.ptr);}
        | FUNCION2_PREDEFINIDA '(' expr ',' expr ')'
                                            {$$=$3;code2(funcion2,(Inst)$1->u.ptr);}
        | '(' expr ')'  	{$$ = $2;}
        | expr '+' expr 	{code(sumar);}
        | expr '-' expr 	{code(restar);}
        | expr '*' expr 	{code(multiplicar);}
        | expr '/' expr 	{code(dividir);}
        | expr CONCATENAR expr {code(concatenar);}
        | expr MODULO expr 	{code(modulo);}
        | expr DIVIDIR_INT expr {code(dividir_int);}
        | expr POTENCIA expr 	{code(potencia);}
        |'-' expr %prec UNARIO 	{$$=$2; code(negativo);}
        |'+' expr %prec UNARIO 	{$$=$2; code(positivo);}
        | expr MAYOR_QUE expr 	{code(mayor_que);}
        | expr MAYOR_IGUAL expr {code(mayor_igual);}
        | expr MENOR_QUE expr 	{code(menor_que);}
        | expr MENOR_IGUAL expr {code(menor_igual);}
        | expr IGUAL expr 	{code(igual);}
        | expr DISTINTO expr 	{code(distinto);}
        | expr Y_LOGICO expr 	{code(y_logico);}
        | expr O_LOGICO expr 	{code(o_logico);}
        | NEGACION expr 	{$$=$2; code(negacion);}
      	;

%%

#include <stdio.h>
#include <ctype.h>
#include <signal.h>
#include <setjmp.h>

/* Depuracion
 1: habilitada
 0: no habilitada
 
 Se debe usar
  bison --debug ...
o
  bison -t
*/
int yydebug=0;

/* Nombre del programa */
char *progname;

/* Contador de líneas */
int lineno = 1;

/* Permite la recuperación de errores */
jmp_buf begin;

/* Dispositivo de entrada estándar de yylex() */
extern FILE * yyin;

/* Indica el modo de ejecución a las funciones del intérprete */
extern int modo_interprete;


int main(int argc, char *argv[])
{

 /* Si se invoca el intérprete con un fichero de entrada */
 /* entonces se establece como dispositivo de entrada para yylex() */
 if (argc == 2) {
  yyin = fopen(argv[1],"r");
  modo_interprete = 0;
 } else {
  modo_interprete = 1;
 }

 progname=argv[0];

 /* inicializacion de la tabla de simbolos */
 init();

/* Establece un estado viable para continuar despues de un error */
 setjmp(begin);

 /* Establece cual va a ser la funcion para tratar errores de punto flotante */
 signal(SIGFPE,fpecatch); /* Excepcion de punto flotante*/

/* initcode inicializa el vector de intrucciones y la pila del interprete */
 for (initcode(); yyparse(); initcode()) execute(prog);

 return 0;

}

void yyerror(char *s)
{
 warning(s,(char *) 0);
}

void warning(char *s, char *t)
{
 fprintf(stderr," ** %s : %s", progname,s);
 if (t) fprintf(stderr," ---> %s ",t);
 fprintf(stderr,"  (linea %d)\n",lineno);
}

void execerror(s,t) /* recuperacion de errores durante la ejecucion */
char *s,*t;
{
 warning(s,t);
  longjmp(begin,0);
}

void fpecatch()     /*  atrapa errores de punto flotante */
{
 execerror("error de punto flotante ",(char *)0);
}

