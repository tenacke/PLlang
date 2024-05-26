%{
#include "defs.h"
#include "parser.tab.h"

void comment();
extern char *yytext;

%}

%option yylineno noyywrap nounput 

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
    yylval.ptr = create_id(yytext);
    return ID; 
}
[0-9]+ { 
    yylval.ptr = create_num(yytext);
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
