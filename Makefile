CC=g++
LEX=flex
YACC=bison
CPPFLAGS=-Wall -Wextra -g -Ofast -fpermissive -std=c++17 
LDFLAGS=
LEXFLAGS=--nomain
BISONFLAGS=-Wcounterexamples -Wall -Wno-precedence -d


mypl: parser.tab.o generator.o lex.yy.o
	$(CC) $(CPPFLAGS) -o $@ lex.yy.o parser.tab.o generator.o $(LDFLAGS)

parser.tab.o: parser.tab.c parser.tab.h
	$(CC) $(CPPFLAGS) -c parser.tab.c

parser.tab.c parser.tab.h: parser.y
	$(YACC) $(BISONFLAGS) parser.y

lex.yy.o: lex.yy.c parser.tab.h
	$(CC) $(CPPFLAGS) -c lex.yy.c

lex.yy.c: lexer.l
	$(LEX) $(LEXFLAGS) lexer.l

generator.o: generator.cpp defs.h generator.hpp
	$(CC) $(CPPFLAGS) -c generator.cpp

clean:
	rm -f mypl lex.yy.c parser.tab.c parser.tab.h *.o *.s *.bc
	if [ -d mypl.dSYM ]; then rm -rf mypl.dSYM; fi
	