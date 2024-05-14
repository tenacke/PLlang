%{
#include <stdio.h>

extern int yylineno;
extern int column;
extern char *lineptr;
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
%left THEN
%right ELSE
%nonassoc EQ NE LT LE GT GE

%define parse.error verbose

%start program

%%
program: block DOT | block error {yyerrok;}
    ;

block: constDecl varDecl funcDecl procDecl statement
    ;

constDecl: CONST constAssList SEMI 
    | CONST constAssList error {yyerrok; }
    |/* empty */
    ;

constAssList: assignment 
    | constAssList COMMA assignment
    | error assignment {yyerrok;}
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
    | error {yyerrok; }
    ;

varDecl: VAR varDeclList SEMI
    | VAR varDeclList error {yyerrok; }
    | /* empty */
    ;

varDeclList: variable
    | varDeclList COMMA variable
    | varDeclList variable error {yyerrok; }
    ;

variable: ID
    | ID LBRACKET expression RBRACKET
    /* TODO: error */
    ;

procDecl: procDecl PROCEDURE ID SEMI block SEMI
    | procDecl PROCEDURE ID SEMI block error {yyerrok; }
    | procDecl PROCEDURE ID error block SEMI  {yyerrok; }
    /* TODO: more errors */
    | /* empty */
    ;

statement: variable ASSIGN expression 
    | CALL ID // procedure call
    | error ID { }
    | BEG statementList END // compound statement
    | IF condition THEN statement %prec THEN
    | IF condition THEN statement ELSE statement 
    | IF condition error { } 
    | WHILE condition DO statement
    | WHILE condition error { }
    | FOR ID ASSIGN expression TO expression DO statement
    | FOR error { }
    | READ LPAREN variable RPAREN { /* read variable */ }
    | WRITE LPAREN variable RPAREN { /* write variable */ }
    | WRITELN LPAREN variable RPAREN { /* write variable with newline */ }
    | BREAK
    | RETURN expression
    | error {  }
    ;

statementList: statement
    | statementList SEMI statement
    | statementList error statement { yyerrok; }
    ;

condition: ODD expression { $$ = $2 % 2; }
    | expression EQ expression { $$ = $1 == $3; }
    | expression NE expression { $$ = $1 != $3; }
    | expression LT expression { $$ = $1 < $3; }
    | expression LE expression { $$ = $1 <= $3; }
    | expression GT expression { $$ = $1 > $3; }
    | expression GE expression { $$ = $1 >= $3; }
    | expression error expression {yyerrok; }
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
    | error { }
    ;

argList: expression 
    | argList COMMA expression
    | argList error expression {yyerrok; }
    ;


%%

void yyerror(const char *s) {
    fprintf(stderr,"error: %s in line %d\n", s, yylineno);
}