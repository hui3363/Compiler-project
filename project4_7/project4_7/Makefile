YACC = bison
YACC_SRC = cm.y
YACC_C = cm.tab.c
YACC_OUT = cm.tab.h cm.output
YACC_OBJ = cm.tab.o

LEX = lex
LEX_SRC = cm.l
LEX_C = lex.yy.c
LEX_OBJ = lex.yy.o

OBJECTS = $(YACC_OBJ) $(LEX_OBJ) main.o util.o analyze.o symtab.o cgen.o code.o
SRCS = $(YACC_C) $(LEX_C) main.c util.c analyze.c symtab.c cgen.c code.c

CC = gcc 
TARGET = project4_7



$(TARGET) : $(OBJECTS)
			  $(CC) -o $(TARGET) $(OBJECTS) -lfl

$(LEX_C) : $(LEX_SRC)
			 $(LEX) $(LEX_SRC)

$(YACC_C) : $(YACC_SRC)
			 $(YACC) -d -v $(YACC_SRC)


all : $(OBJECTS)
			  $(CC) -o $(TARGET) $(OBJECTS) -lfl

clean : 
	rm -f $(OBJECTS) $(TARGET) $(LEX_C) $(YACC_C) $(YACC_OUT) *.s
