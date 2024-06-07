%{
#include "defs.h"
#include <stdio.h>

extern int yylineno;
void yyerror(const char*);
int yylex(void);

%}

%union {
    void *ptr;
}

%define parse.error verbose

%token <ptr> ID NUM
%token IF THEN ELSE WHILE DO FOR BREAK RETURN BEG END TO
%token EQ NE LT LE GT GE
%token ASSIGN SEMI COMMA DOT LPAREN RPAREN LBRACKET RBRACKET LBRACE RBRACE
%token PLUS MINUS TIMES DIVIDE MOD
%token CONST VAR FUNCTION PROCEDURE CALL ODD READ WRITE WRITELN

%nonassoc IFX
%nonassoc ELSE

%type <ptr> block constDecl constAssList constAssign numList funcDecl paramList 
%type <ptr> global_block globalConstDecl globalConstAssList globalConstAssign globalVarDecl globalVarDeclList globalVariableDec
%type <ptr> varDecl varDeclList variableDec procDecl statement statementList 
%type <ptr> condition expression term factor argList



%start program

%%
program: global_block DOT { create_program($1); }
    | global_block error { yyerrok; }
    ;

global_block: globalConstDecl globalVarDecl funcDecl procDecl statement { $$ = create_global_block($1, $2, $3, $4, $5);}
    ;

block: constDecl varDecl statement { $$ = create_simple_block($1, $2, $3); }
    ;

globalConstDecl: CONST globalConstAssList SEMI { $$ = resolve_global_const_assignment($2); }
    | CONST globalConstAssList error { yyerrok; }
    | %empty { $$ = NULL; }
    ;

globalConstAssList: globalConstAssign { $$ = add_global_const_assignment($1); }
    | globalConstAssList COMMA globalConstAssign { $$ = append_global_const_assignment($1, $3); }
    ;

globalConstAssign: ID EQ NUM { $$ = create_global_const_assignment($1, $3); }
    | ID EQ LBRACE numList RBRACE { $$ = create_global_const_array_assignment($1, $4); }
    ;

constDecl: CONST constAssList SEMI { $$ = resolve_local_const_assignment($2); }
    | CONST constAssList error { yyerrok; }
    | %empty { $$ = NULL; }
    ;

constAssList: constAssign { $$ = add_local_const_assignment($1); }
    | constAssList COMMA constAssign { $$ = append_local_const_assignment($1, $3); }
    | error constAssign { yyerrok; }
    ;

constAssign: ID EQ NUM { $$ = create_local_const_assignment($1, $3); }
    | ID EQ LBRACE numList RBRACE { $$ = create_local_const_array_assignment($1, $4); }
    ;

numList: NUM { $$ = add_num_list($1); }
    | numList COMMA NUM { $$ = append_num_list($1, $3); }
    ;

globalVarDecl: VAR globalVarDeclList SEMI { $$ = resolve_global_var_assignment($2); }
    | VAR globalVarDeclList error { yyerrok; }
    | %empty { $$ = NULL; }
    ;

globalVarDeclList: globalVariableDec { $$ = add_global_var_assignment($1); }
    | globalVarDeclList COMMA globalVariableDec { $$ = append_global_var_assignment($1, $3); }
    | globalVarDeclList globalVariableDec error { yyerrok; }
    ;

globalVariableDec: ID { $$ = create_global_var_assignment($1); }
    | ID LBRACKET NUM RBRACKET { $$ = create_global_var_array_assignment($1, $3); }
    ;

varDecl: VAR varDeclList SEMI { $$ = resolve_local_var_assignment($2); }
    | VAR varDeclList error { yyerrok; }
    | %empty { $$ = NULL; }
    ;

varDeclList: variableDec { $$ = add_local_var_assignment($1); }
    | varDeclList COMMA variableDec { $$ = append_local_var_assignment($1, $3); }
    | varDeclList variableDec error { yyerrok; }
    ;

variableDec: ID { $$ = create_local_var_assignment($1); }
    | ID LBRACKET NUM RBRACKET { $$ = create_local_var_array_assignment($1, $3); }
    ;

procDecl: procDecl PROCEDURE ID SEMI block SEMI { $$ = append_proc_decl($1, $3, $5); }
    | procDecl PROCEDURE ID SEMI block error { yyerrok; }
    | procDecl PROCEDURE ID error block SEMI  { yyerrok; }
    | %empty { $$ = NULL; }
    ;

funcDecl: funcDecl FUNCTION ID LPAREN paramList RPAREN SEMI block SEMI { $$ = append_func_decl($1, $3, $5, $8); }
    | funcDecl FUNCTION ID LPAREN paramList RPAREN SEMI block error { yyerrok; }
    | funcDecl FUNCTION ID LPAREN paramList RPAREN error block SEMI  { yyerrok; }
    | %empty { $$ = NULL; }
    ;

paramList: ID { $$ = create_param_list($1); }
    | paramList COMMA ID { $$ = append_param_list($1, $3); }
    | error { }
    ;

statement: ID ASSIGN expression { $$ = create_store_statement($1, $3); }
    | ID LBRACKET expression RBRACKET ASSIGN expression { $$ = create_store_array_statement($1, $3, $6); }
    | CALL ID { $$ = create_call_statement($2); }
    | error ID { }
    | BEG statementList END { $$ = resolve_statement_list($2); }
    | IF condition THEN statement %prec IFX { $$ = create_if_statement($2, $4, NULL); } 
    | IF condition THEN statement ELSE statement { $$ = create_if_statement($2, $4, $6); }
    | IF condition error { } 
    | WHILE condition DO statement { $$ = create_while_statement($2, $4); }
    | WHILE condition error { }
    | FOR ID ASSIGN expression TO expression DO statement { $$ = create_for_statement($2, $4, $6, $8); }
    | FOR error { }
    | READ LPAREN ID RPAREN { $$ = create_read_statement($3); }
    | WRITE LPAREN ID RPAREN { $$ = create_write_statement($3); }
    | WRITELN LPAREN ID RPAREN { $$ = create_writeln_statement($3); }
    | BREAK { $$ = create_break_statement(); }
    | RETURN expression { $$ = create_return_statement($2); }
    | error { }
    ;

statementList: statement { $$ = create_statement_list($1); }
    | statementList SEMI statement { $$ = append_statement_list($1, $3); }
    | statementList error statement { yyerrok; }
    ;

condition: ODD expression { $$ = create_odd_condition($2); }
    | expression EQ expression { $$ = create_eq_condition($1, $3); } 
    | expression NE expression { $$ = create_ne_condition($1, $3); } 
    | expression LT expression { $$ = create_lt_condition($1, $3); } 
    | expression LE expression { $$ = create_le_condition($1, $3); }
    | expression GT expression { $$ = create_gt_condition($1, $3); } 
    | expression GE expression  { $$ = create_ge_condition($1, $3); } 
    | expression error expression { yyerrok; }
    ;

expression: term { $$ = $1; }
    | PLUS term { $$ = $2; }
    | MINUS term { $$ = create_negate_expression($2); }
    | expression PLUS term { $$ = create_add_expression($1, $3); }
    | expression MINUS term { $$ = create_sub_expression($1, $3); }
    ;

term: factor { $$ = $1; }
    | term TIMES factor { $$ = create_mul_expression($1, $3); }
    | term DIVIDE factor { $$ = create_div_expression($1, $3); }
    | term MOD factor { $$ = create_mod_expression($1, $3); }
    ;

factor: ID { $$ = create_id_expression($1); }
    | ID LBRACKET expression RBRACKET { $$ = create_array_expression($1, $3); }
    | NUM { $$ = create_num_expression($1); }
    | LPAREN expression RPAREN { $$ = $2; }
    | ID LPAREN argList RPAREN { $$ = create_call_expression($1, $3); }
    | error { }
    ;

argList: expression { $$ = create_arg_list($1); }
    | argList COMMA expression { $$ = append_arg_list($1, $3); }
    | argList error expression { yyerrok; }
    ;


%%

void yyerror(const char *s) {
    fprintf(stderr,"error: %s in line %d\n", s, yylineno);
    set_error();
}
