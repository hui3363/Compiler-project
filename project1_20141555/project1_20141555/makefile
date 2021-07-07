CC = gcc

OBJECTS = main.o util.o lex.yy.o
TARGET = 20141555

$(TARGET) : $(OBJECTS)
	$(CC) -o $(TARGET) $(OBJECTS)

lex.yy.o: lex.yy.c

lex.yy.c: tiny.l
	flex tiny.l

clean:
	-rm *.o lex.yy.c $(20141555)
