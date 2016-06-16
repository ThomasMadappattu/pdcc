COMPILER := gcc 
CFLAGS   := -Wall -c -pedantic  -g
DELETE   := rm
LIBS     := -lm 
pdcc: io.c codegen.c pdcc.c code16d.c lex.c parser.c error.c symtab.c
	$(COMPILER) $(CFLAGS) io.c -o io.o
	$(COMPILER) $(CFLAGS) codegen.c -o codegen.o 
	$(COMPILER) $(CFLAGS) code16d.c -o code16d.o
	$(COMPILER) $(CFLAGS) lex.c   -o lex.o
	$(COMPILER) $(CFLAGS) pdcc.c -o pdcc.o
	$(COMPILER) $(CFLAGS) parser.c -o parser.o
	$(COMPILER) $(CFLAGS) error.c -o error.o
	$(COMPILER) $(CFLAGS) symtab.c -o symtab.o
	$(COMPILER) io.o codegen.o pdcc.o code16d.o lex.o parser.o error.o symtab.o $(LIBS) -o pdcc

clean:
	$(DELETE) *.o
	$(DELETE) pdcc
