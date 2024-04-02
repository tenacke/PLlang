%{
#include <stdio.h>

extern int yylineno;

void yyerror(const char *s);
int yylex(void);


%}

%token ID NUM
%token IF THEN ELSE WHILE DO FOR BREAK RETURN BEG END TO
%token EQ NE LT LE GT GE
%token ASSIGN SEMI COMMA DOT LPAREN RPAREN LBRACKET RBRACKET LBRACE RBRACE
%token PLUS MINUS TIMES DIVIDE MOD
%token CONST VAR FUNCTION PROCEDURE CALL ODD READ WRITE WRITELN

%left PLUS MINUS
%left TIMES DIVIDE MOD
%nonassoc EQ NE LT LE GT GE


%start program

%%
program: block DOT
    ;

block: constDecl varDecl funcDecl procDecl statement
    ;

constDecl: CONST constAssList SEMI 
    | /* empty */
    ;

constAssList: assignment 
    | constAssList COMMA assignment
    ;

assignment: ID EQ NUM 
    | ID EQ LBRACE numList RBRACE
    ;

numList: NUM
    | numList COMMA NUM
    ;

funcDecl: funcDecl FUNCTION ID LPAREN paramList RPAREN SEMI block SEMI
    | /* empty */
    ;

paramList: ID
    | paramList COMMA ID
    ;

varDecl: VAR varDeclList SEMI
    | /* empty */
    ;

varDeclList: variable
    | varDeclList COMMA variable
    ;

variable: ID
    | ID LBRACKET expression RBRACKET
    ;

procDecl: procDecl PROCEDURE ID SEMI block SEMI
    | /* empty */
    ;

statement: variable ASSIGN expression 
    | CALL ID // procedure call
    | BEG statementList END // compound statement
    | IF condition THEN statement 
    | IF condition THEN statement ELSE statement 
    | WHILE condition DO statement
    | FOR ID ASSIGN expression TO expression DO statement
    | READ LPAREN variable RPAREN { /* read variable */ }
    | WRITE LPAREN variable RPAREN { /* write variable */ }
    | WRITELN LPAREN variable RPAREN { /* write variable with newline */ }
    | BREAK
    | RETURN expression
    //| error RPAREN { yyerrok; } // error recovery but use wisely
    ;

statementList: statement
    | statementList SEMI statement
    //| error SEMI { yyerrok; }
    ;

condition: ODD expression { $$ = $2 % 2; }
    | expression EQ expression { $$ = $1 == $3; }
    | expression NE expression { $$ = $1 != $3; }
    | expression LT expression { $$ = $1 < $3; }
    | expression LE expression { $$ = $1 <= $3; }
    | expression GT expression { $$ = $1 > $3; }
    | expression GE expression { $$ = $1 >= $3; }
    ;

expression: term { $$ = $1; }
    | PLUS term { $$ = $2; }
    | MINUS term { $$ = -$2; }
    | expression PLUS term { $$ = $1 + $3; }
    | expression MINUS term { $$ = $1 - $3; }
    ;

term: factor { $$ = $1; }
    | term TIMES factor { $$ = $1 * $3; }
    | term DIVIDE factor { $$ = $1 / $3; }
    | term MOD factor { $$ = $1 % $3; }
    ;

factor: variable { $$ = $1; }
    | NUM { $$ = $1; }
    | LPAREN expression RPAREN { $$ = $2; }
    | ID LPAREN argList RPAREN { /* function call */ }
    ;

argList: expression
    | argList COMMA expression
    ;


%%

void yyerror(const char *s) {
    fprintf(stderr, "%s at %d\n", s, yylineno);
}