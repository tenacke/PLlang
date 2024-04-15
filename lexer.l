%{
#include <stdio.h>
#include "y.tab.h"

void comment();
extern FILE *yyin;
extern int yylineno;
extern int yyparse();
extern int yywrap();
int main(int argc, char *argv[]);

%}

%%
"/*" { comment(); }

"const" { return CONST; }
"var" { return VAR; }
"procedure" { return PROCEDURE; }
"function" { return FUNCTION; }
"call" { return CALL; }
"begin" { return BEG; }
"end" { return END; }
"if" { return IF; }
"then" { return THEN; }
"else" { return ELSE; }
"while" { return WHILE; }
"do" { return DO; }
"for" { return FOR; }
"to" { return TO; }
"break" { return BREAK; }
"return" { return RETURN; }
"odd" { return ODD; }
"read" { return READ; }
"write" { return WRITE; }
"writeline" { return WRITELN; }
"[" { return LBRACKET; }
"]" { return RBRACKET; }
"(" { return LPAREN; }
")" { return RPAREN; }
"{" { return LBRACE; }
"}" { return RBRACE; }
"+" { return PLUS; }
"-" { return MINUS; }
"*" { return TIMES; }
"/" { return DIVIDE; }
"%" { return MOD; }
"=" { return EQ; }
"<" { return LT; }
"<=" { return LE; }
">" { return GT; }
">=" { return GE; }
"<>" { return NE; }
":=" { return ASSIGN; }
";" { return SEMI; }
"," { return COMMA; }
"." { return DOT; }
[a-zA-Z_][a-zA-Z0-9_]* { return ID; }
[0-9]+ { return NUM; }
\n { yylineno++; }
[ \t] { }
^[a-zA-Z0-9_\.,;:=<>\+\-\*%\[\]\{\}\(\)] { printf("Error: Unknown character at line %d: %s\n", yylineno, yytext); }
%%

int yywrap() 
{
	return(1);
}

void comment() 
{
    int c;
    while ((c = input()) != EOF) {
        if (c == '*') {
            c = input();
            if (c == '/') 
                return;
        }
    }
    exit(1);
}

int main(int argc, char *argv[]) 
{
    if (argc < 2) {
        printf("Usage: %s <filename>\n", argv[0]);
        return 1;
    }
    yyin = fopen(argv[1], "r");
    yyparse();
    fclose(yyin);
}
