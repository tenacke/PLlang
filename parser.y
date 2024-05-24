%{
#include <cstdio>
#include <iostream>
#include <string>
#include <cstring>
#include "mylib.h"

extern int yylineno;
extern int column;
void yyerror(const char *s);
int yylex(void);
char *append(char* old, char *s);

using namespace std;

%}

%union {
    const char *str;
}

%token <str> ID NUM
%token <str> IF THEN ELSE WHILE DO FOR BREAK RETURN BEG END TO
%token <str> EQ NE LT LE GT GE
%token <str> ASSIGN SEMI COMMA DOT LPAREN RPAREN LBRACKET RBRACKET LBRACE RBRACE
%token <str> PLUS MINUS TIMES DIVIDE MOD
%token <str> CONST VAR FUNCTION PROCEDURE CALL ODD READ WRITE WRITELN

%nonassoc IFX
%nonassoc ELSE

%type <str> block constDecl constAssList constAssign numList funcDecl paramList 
%type <str> global_block globalConstDecl globalConstAssList globalConstAssign globalVarDecl globalVarDeclList globalVariableDec
%type <str> varDecl varDeclList variableDec procDecl statement statementList 
%type <str> condition expression term factor argList variable



%start program

%%
program: global_block DOT { generate_llvm_code(string($1)); }
    | global_block error { yyerrok; }
    ;

global_block: globalConstDecl globalVarDecl funcDecl procDecl statement { $$ = $5; }
    ;

block: constDecl varDecl statement { $$ = $3; }
    ;

globalConstDecl: CONST globalConstAssList SEMI { }
    | CONST globalConstAssList error { yyerrok; }
    | { }
    ;

globalConstAssList: globalConstAssign { }
    | globalConstAssList COMMA globalConstAssign { }
    ;

globalConstAssign: ID EQ NUM { declare_global_constant($1, $3); }
    | ID EQ LBRACE numList RBRACE { declare_global_constant_array($1, $4); }
    ;

constDecl: CONST constAssList SEMI { }
    | CONST constAssList error { yyerrok; }
    | { }
    ;

constAssList: constAssign { }
    | constAssList COMMA constAssign { }
    | error constAssign { yyerrok; }
    ;

constAssign: ID EQ NUM { declare_local_constant($1, $3); }
    | ID EQ LBRACE numList RBRACE { declare_local_constant_array($1, $4);}
    ;

numList: NUM { }
    | numList COMMA NUM { }
    ;

globalVarDecl: VAR globalVarDeclList SEMI { }
    | VAR globalVarDeclList error { yyerrok; }
    | { }
    ;

globalVarDeclList: globalVariableDec { }
    | globalVarDeclList COMMA globalVariableDec { }
    | globalVarDeclList globalVariableDec error { yyerrok; }
    ;

globalVariableDec: ID { declare_global_variable($1); }
    | ID LBRACKET NUM RBRACKET { declare_global_variable_array($1, $3);}
    ;

varDecl: VAR varDeclList SEMI { }
    | VAR varDeclList error { yyerrok; }
    | { }
    ;

varDeclList: variableDec { }
    | varDeclList COMMA variableDec { }
    | varDeclList variableDec error { yyerrok; }
    ;

variableDec: ID { declare_local_variable($1); }
    | ID LBRACKET NUM RBRACKET { declare_local_variable_array($1, $3); }
    ;

procDecl: procDecl PROCEDURE ID SEMI block SEMI { generate_procedure($3, string($5)); }
    | procDecl PROCEDURE ID SEMI block error { yyerrok; }
    | procDecl PROCEDURE ID error block SEMI  { yyerrok; }
    | { }
    ;

funcDecl: funcDecl FUNCTION ID LPAREN paramList RPAREN SEMI block SEMI { generate_function($3, $5, string($8));}
    | funcDecl FUNCTION ID LPAREN paramList RPAREN SEMI block error { yyerrok; }
    | funcDecl FUNCTION ID LPAREN paramList RPAREN error block SEMI  { yyerrok; }
    | { }
    ;

paramList: ID { }
    | paramList COMMA ID { }
    | error { yyerrok; }
    ;

statement: variable ASSIGN expression { $$ = strdup(store_variable($1, $3).c_str());}
    | CALL ID { $$ = strdup(procedure_call($2).c_str()); }
    | error ID { }
    | BEG statementList END { $$ = $2; }
    | IF condition THEN statement %prec IFX { $$ = strdup(if_then(string($2), string($4)).c_str()); } 
    | IF condition THEN statement ELSE statement { $$ = strdup(if_else(string($2), string($4), string($6)).c_str()); }
    | IF condition error { } 
    | WHILE condition DO statement { $$ = strdup(while_loop(string($2), $4).c_str()); }
    | WHILE condition error { }
    | FOR ID ASSIGN expression TO expression DO statement { }
    | FOR error { }
    | READ LPAREN variable RPAREN { $$ = strdup(read_variable($3).c_str()); }
    | WRITE LPAREN variable RPAREN { $$ = strdup(write_variable($3).c_str()); }
    | WRITELN LPAREN variable RPAREN { $$ = strdup(writeln_variable($3).c_str()); }
    | BREAK { $$ = strdup(break_loop().c_str()); }
    | RETURN expression { $$ = strdup(return_value($2).c_str()); }
    | error {  }
    ;

statementList: statement { $$ = $1; }
    | statementList SEMI statement { $$ = strdup((string($1) + "\n" + string($3)).c_str());}
    | statementList error statement { yyerrok; }
    ;

condition: ODD expression { add_condition(do_operation("srem", $2, "2")); $$ = strdup(wrap_expression().c_str()); }
    | expression EQ expression { add_condition(do_operation("icmp eq", $1, $3)); $$ = strdup(wrap_expression().c_str()); } 
    | expression NE expression { add_condition(do_operation("icmp ne", $1, $3)); $$ = strdup(wrap_expression().c_str()); } 
    | expression LT expression { add_condition(do_operation("icmp slt", $1, $3)); $$ = strdup(wrap_expression().c_str()); } 
    | expression LE expression { add_condition(do_operation("icmp sle", $1, $3)); $$ = strdup(wrap_expression().c_str()); }
    | expression GT expression { add_condition(do_operation("icmp sgt", $1, $3)); $$ = strdup(wrap_expression().c_str()); } 
    | expression GE expression  { add_condition(do_operation("icmp sge", $1, $3)); $$ = strdup(wrap_expression().c_str()); } 
    | expression error expression { yyerrok; }
    ;

expression: term { $$ = $1; }
    | PLUS term { $$ = $2; }
    | MINUS term { $$ = do_operation("sub", "0", $2); }
    | expression PLUS term { $$ = do_operation("add", $1, $3); }
    | expression MINUS term { $$ = do_operation("sub", $1, $3); }
    ;

term: factor { $$ = $1;}
    | term TIMES factor { $$ = do_operation("mul", $1, $3); }
    | term DIVIDE factor { $$ = do_operation("div", $1, $3); }
    | term MOD factor { $$ = do_operation("srem", $1, $3); }
    ;

factor: variable { $$ = load_variable($1); }
    | NUM { $$ = $1; }
    | LPAREN expression RPAREN { $$ = $2; }
    | ID LPAREN argList RPAREN { $$ = function_call($1, $3); }
    | error { }
    ;

variable: ID { $$ = $1; }
    | ID LBRACKET expression RBRACKET { $$ = $1 }
    ;

argList: expression { }
    | argList COMMA expression { }
    | argList error expression { yyerrok; }
    ;


%%

void yyerror(const char *s) {
    fprintf(stderr,"error: %s in line %d\n", s, yylineno);
}

char *append(char* old, char *s) {
    string str = string(old);
    str += "\n";
    str += s;
    return strdup(str.c_str());
}