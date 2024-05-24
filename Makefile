CC=g++
CFLAGS=-Wall -Wextra -g -Ofast -fpermissive -std=c++11
LDFLAGS=-ll -mmacosx-version-min=14.4
BISONFLAGS=-dy -Dparse.error=verbose -Wcounterexamples -Wall -Wno-empty-rule -Wno-precedence


mypl: mylib.o y.tab.o  lex.yy.o
	$(CC) -o mypl lex.yy.o y.tab.o mylib.o $(LDFLAGS)

y.tab.o: y.tab.c
	$(CC) $(CFLAGS) -c y.tab.c

lex.yy.o: lex.yy.c
	$(CC) $(CFLAGS) -c lex.yy.c

lex.yy.c: lexer.l y.tab.c
	flex lexer.l

y.tab.c: parser.y
	bison $(BISONFLAGS) parser.y

mylib.o: mylib.cpp mylib.h
	$(CC) $(CFLAGS) -c mylib.cpp

clean:
	rm -f mypl lex.yy.c y.tab.c y.tab.h *.o
	if [ -d mypl.dSYM ]; then rm -r mypl.dSYM; fi
