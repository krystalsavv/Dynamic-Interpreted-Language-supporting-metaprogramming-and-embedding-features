
/* A Bison parser, made by GNU Bison 2.4.1.  */

/* Skeleton interface for Bison's Yacc-like parsers in C
   
      Copyright (C) 1984, 1989, 1990, 2000, 2001, 2002, 2003, 2004, 2005, 2006
   Free Software Foundation, Inc.
   
   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.
   
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
   
   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

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


/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     IDENT = 258,
     INTEGER = 259,
     REALNUMBER = 260,
     STRING = 261,
     MULTI_LINE_COMMENT = 262,
     NESTED_COMMENT = 263,
     WHILE = 264,
     FOR = 265,
     IF = 266,
     ELSE = 267,
     FUNCTION = 268,
     RETURN = 269,
     BREAK = 270,
     CONTINUE = 271,
     AND = 272,
     OR = 273,
     NOT = 274,
     LOCAL = 275,
     TRUE = 276,
     FALSE = 277,
     NIL = 278,
     PLUS = 279,
     MINUS = 280,
     UMINUS = 281,
     ASSIGN = 282,
     MULTI = 283,
     MOD = 284,
     DIV = 285,
     EQUAL = 286,
     NOT_EQUAL = 287,
     INCREMENT = 288,
     DECREMENT = 289,
     GREATER_OR_EQUAL = 290,
     LESS_OR_EQUAL = 291,
     GREATER = 292,
     LESS = 293,
     LEFT_SQUARE_BRACKET = 294,
     RIGHT_SQUARE_BRACKET = 295,
     LEFT_BRACKET = 296,
     RIGHT_BRACKET = 297,
     LEFT_PARENTHESIS = 298,
     RIGHT_PARENTHESIS = 299,
     COMMA = 300,
     SEMICOLON = 301,
     COLON = 302,
     DOT = 303,
     DOUBLE_DOT = 304,
     SCOPE = 305,
     WRONG_DEFINITION = 306
   };
#endif
/* Tokens.  */
#define IDENT 258
#define INTEGER 259
#define REALNUMBER 260
#define STRING 261
#define MULTI_LINE_COMMENT 262
#define NESTED_COMMENT 263
#define WHILE 264
#define FOR 265
#define IF 266
#define ELSE 267
#define FUNCTION 268
#define RETURN 269
#define BREAK 270
#define CONTINUE 271
#define AND 272
#define OR 273
#define NOT 274
#define LOCAL 275
#define TRUE 276
#define FALSE 277
#define NIL 278
#define PLUS 279
#define MINUS 280
#define UMINUS 281
#define ASSIGN 282
#define MULTI 283
#define MOD 284
#define DIV 285
#define EQUAL 286
#define NOT_EQUAL 287
#define INCREMENT 288
#define DECREMENT 289
#define GREATER_OR_EQUAL 290
#define LESS_OR_EQUAL 291
#define GREATER 292
#define LESS 293
#define LEFT_SQUARE_BRACKET 294
#define RIGHT_SQUARE_BRACKET 295
#define LEFT_BRACKET 296
#define RIGHT_BRACKET 297
#define LEFT_PARENTHESIS 298
#define RIGHT_PARENTHESIS 299
#define COMMA 300
#define SEMICOLON 301
#define COLON 302
#define DOT 303
#define DOUBLE_DOT 304
#define SCOPE 305
#define WRONG_DEFINITION 306




#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
{

/* Line 1676 of yacc.c  */
#line 16 "parser.y"

	std::string stringVal;
	int intVal;
	double doubleVal;



/* Line 1676 of yacc.c  */
#line 162 "parser.hpp"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif

extern YYSTYPE yylval;


