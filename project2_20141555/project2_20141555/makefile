TARGET = 20141555

CC = gcc
CFLAGS = -g -c -Wall

OBJECTS = main.o util.o lex.yy.o y.tab.o
SRCS = main.c util.c lex.yy.c y.tab.c


$(TARGET) : $(OBJECTS) lex.yy.c y.tab.c
	$(CC) -o $(TARGET) $(OBJECTS)

lex.yy.c: tiny.l
	flex tiny.l

y.tab.c : tiny.y
	yacc -d tiny.y

main.o : util.h globals.h parse.h $(SRCS)
	$(CC) $(CFLAGS) -c $(SRCS)



clean:
	-rm -rf *.o lex.yy.c y.tab.* 20141555
