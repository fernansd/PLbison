/*!
	\file ejemplo9.h 
	\brief Prototipos de funciones auxiliares
*/

#ifndef EJEMPLO_H
#define EJEMPLO_H

/* Analizador léxico */
int yylex();

/* Informa de la detección de un error sintáctico */
void yyerror(char *s);

/* Recuperacion de errores durante la ejecucion */
void execerror(char *s,char *t);

/*  Escribe por pantalla información sobre un error sintáctico */
void warning(char *s, char *t);

/* Captura de errores de punto flotante */
void fpecatch();

/* Inicializa la tabla de símbolos */
void init();

typedef struct Symbol { /* entrada en la tabla de simbolos */
	               char *nombre;
	               short tipo;  /* VAR, FUNCION, INDEFINIDA */
	               short subtipo; /* VSTR, VNUM */
	               struct {
		              double val;        /* si es VAR NUMBER */
	               	  char *str; /* si es VAR STRING */
		              double (*ptr)();   /* si es FUNCION */
		            } u;
                       struct Symbol * siguiente;
	              } Symbol;


typedef union Installval { /* valor para la función install */
					char *str; /* si es STRING */
					double num; /* si es NUMBER */
				  } Installval;

/* Instala en la tabla de símbolos */
Symbol *install(char *s, int tipo, int subtipo, Installval v);

/* Busca en la tabla de símbolos */
Symbol *lookup(char *s);

typedef struct Datum { /* tipo de la pila del interprete */ 
                     double val;
                     Symbol *sym;
                     char *str;
                     short subtipo; /* si es STRING o NUMBER */
                    } Datum;


/* Función que introduce un dato de la pila de valores */
void push(Datum d);

/* Funciones que sacan un dato de la pila de valores */
extern Datum pop();
extern void pop2();

typedef void (*Inst)(); /* instruccion maquina */
#define STOP (Inst) 0

/* Vector de instrucciones */
extern Inst prog[];

/* NOVEDAD */
extern Inst* progp;


/* Función que inicializa el vector de instrucciones */
void initcode();

/* Función que inserta una instucción en el vector de instrucciones */
Inst *code(Inst f);

/* Función que ejecuta una función del vector de instrucciones */
void execute(Inst *p);

/* Prototipos de funciones auxiliares */
extern void assign();
extern void constpush();
void dividir();
void dividir_int();
void escribir();
void eval();


void funcion0();
void funcion1();
void funcion2();


void modulo();
void multiplicar();
void negativo();
void positivo();
void potencia(); 
void restar();
void sumar();
void varpush();


void ifcode();
void whilecode();
void repeatcode();
void forcode();

void mayor_que();
void menor_que();
void mayor_igual();
void menor_igual();
void igual(); 
void distinto();
void y_logico();
void o_logico();
void negacion();

void leervariable();
void leercadena();

#endif
