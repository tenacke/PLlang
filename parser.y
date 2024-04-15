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
%left THEN
%right ELSE
%nonassoc EQ NE LT LE GT GE


%start program

%%
program: block DOT | block error {yyerrok; printf("Dot expected\n");}
    ;

block: constDecl varDecl funcDecl procDecl statement
    ;

constDecl: CONST constAssList SEMI 
    | CONST constAssList error {yyerrok; printf("Semicolon expected\n");}
    |/* empty */
    ;

constAssList: assignment 
    | constAssList COMMA assignment
    | error assignment {yyerrok; printf("Comma expected\n");}
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
    | error {yyerrok; printf("Comma expected\n");}
    ;

varDecl: VAR varDeclList SEMI
    | VAR varDeclList error {yyerrok; printf("Semicolon expected\n");}
    | /* empty */
    ;

varDeclList: variable
    | varDeclList COMMA variable
    | varDeclList variable error {yyerrok; printf("Comma expected\n");}
    ;

variable: ID
    | ID LBRACKET expression RBRACKET
    /* TODO: error */
    ;

procDecl: procDecl PROCEDURE ID SEMI block SEMI
    | procDecl PROCEDURE ID SEMI block error {yyerrok; printf("Semicolon expected after procedure definition\n");}
    | procDecl PROCEDURE ID error block SEMI  {yyerrok; printf("Semicolon expected before code block\n");}
    /* TODO: more errors */
    | /* empty */
    ;

statement: variable ASSIGN expression 
    | CALL ID // procedure call
    | BEG statementList END // compound statement
    | IF condition THEN statement %prec THEN
    | IF condition THEN statement ELSE statement 
    | WHILE condition DO statement
    | FOR ID ASSIGN expression TO expression DO statement
    | READ LPAREN variable RPAREN { /* read variable */ }
    | WRITE LPAREN variable RPAREN { /* write variable */ }
    | WRITELN LPAREN variable RPAREN { /* write variable with newline */ }
    | BREAK
    | RETURN expression
    | error {yyerrok; printf("Invalid statement\n");}
    ;

statementList: statement
    | statementList SEMI statement
    | statementList error statement { yyerrok; printf("Semicolon expected\n");}
    ;

condition: ODD expression { $$ = $2 % 2; }
    | expression EQ expression { $$ = $1 == $3; }
    | expression NE expression { $$ = $1 != $3; }
    | expression LT expression { $$ = $1 < $3; }
    | expression LE expression { $$ = $1 <= $3; }
    | expression GT expression { $$ = $1 > $3; }
    | expression GE expression { $$ = $1 >= $3; }
    | expression error expression {yyerrok; printf("Comparation expected\n");}
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
    | error {yyerrok; printf("Factor expected\n");}
    ;

argList: expression 
    | argList COMMA expression
    | argList error expression {yyerrok; printf("Comma expected\n"); }
    ;


%%

void yyerror(const char *s) {
    fprintf(stderr, "%s at %d: ", s, yylineno);
}