VERSION= 1
FUENTE = ejemplo$(VERSION)
LEXICO = lexico$(VERSION)

CC = gcc
CFLAGS = -c -g -Wall -ansi
YFLAGS = -d         # Generar el fichero $(FUENTE).tab.h 
LFLAGS = -lm -lfl   # fl: biblioteca de flex; m: biblioteca matemática  
OBJS= $(FUENTE).tab.o lex.yy.o  init$(VERSION).o   math$(VERSION).o   symbol$(VERSION).o  code$(VERSION).o

$(FUENTE).exe: $(OBJS) 
	$(CC) $(OBJS) $(LFLAGS) -o $(FUENTE).exe

code$(VERSION).o:  code$(VERSION).c $(FUENTE).h
	$(CC) -c $(CFLAGS) code$(VERSION).c

init$(VERSION).o:  init$(VERSION).c $(FUENTE).h $(FUENTE).tab.h
	$(CC) -c $(CFLAGS) init$(VERSION).c

symbol$(VERSION).o:  symbol$(VERSION).c $(FUENTE).h $(FUENTE).tab.h
	$(CC) -c $(CFLAGS) symbol$(VERSION).c

math$(VERSION).o:  math$(VERSION).c $(FUENTE).h
	$(CC) -c $(CFLAGS) math$(VERSION).c

lex.yy.o:  lex.yy.c $(FUENTE).tab.h $(FUENTE).h macros.h
	$(CC) -c lex.yy.c

lex.yy.c: $(LEXICO).l $(FUENTE).tab.h $(FUENTE).h macros.h
	flex $(LEXICO).l

$(FUENTE).tab.o:  $(FUENTE).tab.c $(FUENTE).tab.h $(FUENTE).h
	$(CC) -c $(CFLAGS) $(FUENTE).tab.c

$(FUENTE).tab.c $(FUENTE).tab.h:  $(FUENTE).y $(FUENTE).h 
	bison $(YFLAGS) $(FUENTE).y

#Opcion para borrar los ficheros objetos y auxiliares
clean: 
	rm -f  $(OBJS) $(FUENTE).tab.[ch] lex.yy.c $(FUENTE).exe $(FUENTE).output
