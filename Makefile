CC=gcc
CFLAGS=-Wall -Wextra -g -O3 
LDFLAGS=-ll


plcc: lex.yy.c y.tab.c
	$(CC) $(CFLAGS) -o plcc lex.yy.c y.tab.c $(LDFLAGS)

lex.yy.c: lexer.l
	flex lexer.l

y.tab.c: parser.y
	bison -dy parser.y -Wcounterexamples

clean:
	rm -f lex.yy.c y.tab.c y.tab.h plcc
