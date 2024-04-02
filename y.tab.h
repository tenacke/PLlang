/* A Bison parser, made by GNU Bison 3.8.2.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2021 Free Software Foundation,
   Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <https://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

#ifndef YY_YY_Y_TAB_H_INCLUDED
# define YY_YY_Y_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token kinds.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    YYEMPTY = -2,
    YYEOF = 0,                     /* "end of file"  */
    YYerror = 256,                 /* error  */
    YYUNDEF = 257,                 /* "invalid token"  */
    ID = 258,                      /* ID  */
    NUM = 259,                     /* NUM  */
    IF = 260,                      /* IF  */
    THEN = 261,                    /* THEN  */
    ELSE = 262,                    /* ELSE  */
    WHILE = 263,                   /* WHILE  */
    DO = 264,                      /* DO  */
    FOR = 265,                     /* FOR  */
    BREAK = 266,                   /* BREAK  */
    RETURN = 267,                  /* RETURN  */
    BEG = 268,                     /* BEG  */
    END = 269,                     /* END  */
    TO = 270,                      /* TO  */
    EQ = 271,                      /* EQ  */
    NE = 272,                      /* NE  */
    LT = 273,                      /* LT  */
    LE = 274,                      /* LE  */
    GT = 275,                      /* GT  */
    GE = 276,                      /* GE  */
    ASSIGN = 277,                  /* ASSIGN  */
    SEMI = 278,                    /* SEMI  */
    COMMA = 279,                   /* COMMA  */
    DOT = 280,                     /* DOT  */
    LPAREN = 281,                  /* LPAREN  */
    RPAREN = 282,                  /* RPAREN  */
    LBRACKET = 283,                /* LBRACKET  */
    RBRACKET = 284,                /* RBRACKET  */
    LBRACE = 285,                  /* LBRACE  */
    RBRACE = 286,                  /* RBRACE  */
    PLUS = 287,                    /* PLUS  */
    MINUS = 288,                   /* MINUS  */
    TIMES = 289,                   /* TIMES  */
    DIVIDE = 290,                  /* DIVIDE  */
    MOD = 291,                     /* MOD  */
    CONST = 292,                   /* CONST  */
    VAR = 293,                     /* VAR  */
    FUNCTION = 294,                /* FUNCTION  */
    PROCEDURE = 295,               /* PROCEDURE  */
    CALL = 296,                    /* CALL  */
    ODD = 297,                     /* ODD  */
    READ = 298,                    /* READ  */
    WRITE = 299,                   /* WRITE  */
    WRITELN = 300                  /* WRITELN  */
  };
  typedef enum yytokentype yytoken_kind_t;
#endif
/* Token kinds.  */
#define YYEMPTY -2
#define YYEOF 0
#define YYerror 256
#define YYUNDEF 257
#define ID 258
#define NUM 259
#define IF 260
#define THEN 261
#define ELSE 262
#define WHILE 263
#define DO 264
#define FOR 265
#define BREAK 266
#define RETURN 267
#define BEG 268
#define END 269
#define TO 270
#define EQ 271
#define NE 272
#define LT 273
#define LE 274
#define GT 275
#define GE 276
#define ASSIGN 277
#define SEMI 278
#define COMMA 279
#define DOT 280
#define LPAREN 281
#define RPAREN 282
#define LBRACKET 283
#define RBRACKET 284
#define LBRACE 285
#define RBRACE 286
#define PLUS 287
#define MINUS 288
#define TIMES 289
#define DIVIDE 290
#define MOD 291
#define CONST 292
#define VAR 293
#define FUNCTION 294
#define PROCEDURE 295
#define CALL 296
#define ODD 297
#define READ 298
#define WRITE 299
#define WRITELN 300

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef int YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;


int yyparse (void);


#endif /* !YY_YY_Y_TAB_H_INCLUDED  */
