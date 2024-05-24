%{
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include "y.tab.h"

void comment();
extern FILE *yyin;
extern FILE *yyout;
extern int yylineno;
extern char *yytext;
extern int yyparse();
int main(int argc, char *argv[]);

%}

%option yylineno yywrap nounput 

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
[a-zA-Z_][a-zA-Z0-9_]* { 
    yylval.str = strdup(yytext); 
    return ID; 
}
[0-9]+ { 
    yylval.str = strdup(yytext); 
    return NUM; 
}
[ \t\n] { }
^[a-zA-Z0-9_\.,;:=<>\+\-\*%\[\]\{\}\(\)] { printf("Error: Unknown character at line %d: %s\n", yylineno, yytext); }
%%

void comment() 
{
    int c;
    while ((c = yyinput()) != EOF) {
        if (c == '*') {
            c = yyinput();
            if (c == '/') 
                return;
        }
    }
    exit(1);
}

int main(int argc, char *argv[]) 
{
    // Usage: mypl [-O] <input_file>
    if (argc < 2) {
        printf("Usage: %s <input_file>\n", argv[0]);
        exit(1);
    } else if (argc == 3 && strcmp(argv[1], "-O") != 0) {
        printf("Usage: %s <input_file>\n", argv[0]);
        exit(1);
    } else if (argc == 3) {
        yyin = fopen(argv[2], "r");
    } else {
        yyin = fopen(argv[1], "r");
    }
    yyparse();
    fclose(yyin);
}
