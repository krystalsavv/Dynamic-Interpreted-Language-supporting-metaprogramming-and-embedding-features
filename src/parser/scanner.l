%{
	#pragma warning (disable : 4005)
	#include <stdio.h>
	#include <stdlib.h>
	#include <string>
	#include <iostream>
	#include "parser.hpp"

	int keywords_to_enum(std::string text);
	std::string stringVal = "";
	int commentCounter = 0; 
%}

%option noyywrap
%option yylineno
%option case-insensitive
%option reentrant
%option header-file="generated/scanner.h"
%option bison-bridge

keyword 	(function)|(while)|(for)|(if)|(else)|(continue)|(break)|(return)|(and)|(or)|(not)|(local)|(true)|(false)|(nil)
operator 	[\*\/\%]|(\={1,2})|(\>\=?)|(\<\=?)|(\!\=)|(\+{1,2})|(\-{1,2})
integer 	(0[xX][0-9A-Fa-f]+)|([0-9]+)
real 		[0-9]+\.[0-9](e|E)?[0-9]*	
space 		[\r\n \t\v]
id 			[a-zA-Z][a-zA-Z_0-9]* 
comment1 	"//".*
punc 		[\[\]\{\}\(\)\;\,]|(\:{1,2})|(\.{1,2})

%x STRING_
%x COMMENT2

%%

"\.\<"						{ return META_SYNTAX_OPEN; }
"\>\."						{ return META_SYNTAX_CLOSE; }
"\.~"						{ return META_ESCAPE; }
"\.\!"						{ return META_INLINE; }
"\.@"						{ return META_PARSE; }
"\.#"						{ return META_UNPARSE; }

"\"" 						{
								stringVal = "";
								BEGIN(STRING_);
							}

<STRING_>"\""				{
								yylval->stringVal = new std::string(stringVal); 
								std::cout << stringVal << std::endl;
								BEGIN(INITIAL);
								return STRING;
							}

<STRING_>"\n"|"\\n"			{

								stringVal += "\n";
							}

		
<STRING_>"\\t" 				{
								stringVal += "\t";
							}	

<STRING_>"\\r" 				{
								stringVal += "\r";
							}					

<STRING_>"\\\"" 	{
								stringVal += "\"";
							}

<STRING_>"\\\\" 	{
								stringVal += "\\";
							}


<STRING_>"\\"				{ std::cerr << "Error: invalid escape character "<< yytext << " at line "<< yylineno << " .\n"; yyterminate(); }
<STRING_><<EOF>> 			{ std::cerr << "Error: String is not closed at line " << yylineno << std::endl; yyterminate(); } 	

<STRING_>.					{ stringVal += yytext; }


"/*" 						{ commentCounter++; BEGIN(COMMENT2); }

<COMMENT2>"/*" 				{ commentCounter++; }

<COMMENT2>"*/" 				{
								commentCounter--;
								if(!commentCounter)
									BEGIN(INITIAL);
							}

<COMMENT2><<EOF>>  			{ std::cerr << "Error: Comment is not closed (EOF) started at line " << yylineno << std::endl; yyterminate(); } 

<COMMENT2>. 				{}




{keyword}	{return keywords_to_enum(yytext);}
{operator}	{return keywords_to_enum(yytext);}
{integer} 	{	yylval->numVal = atoi(yytext);
				return INTEGER;}
{real} 		{	yylval->numVal = atof(yytext);
				return REALNUMBER;
			}
{space}		{}
{comment1}	{}
{punc}		{return keywords_to_enum(yytext);}
{id}		{	stringVal.assign(yytext); 
				yylval->stringVal = new std::string(stringVal); 
				return IDENT; }

. 			{std::cerr << "Error: Undefined token \"" << yytext << "\" at line " << yylineno << ". \n"; yyterminate();}

%%

int keywords_to_enum(std::string text) {
	if(text == "if") 			return IF;
	else if(text == "else") 	return ELSE;
	else if(text == "while") 	return WHILE;
	else if(text == "for") 		return FOR;
	else if(text == "function") return FUNCTION;
	else if(text == "return")	 return RETURN;
	else if(text == "break") 	return BREAK;
	else if(text == "continue") return CONTINUE;
	else if(text == "and")		return AND;
	else if(text == "not")		return NOT;
	else if(text == "or") 		return OR;	
	else if(text == "local") 	return LOCAL;
	else if(text == "true") 	return TRUE;
	else if(text == "false") 	return FALSE;
	else if(text == "nil") 		return NIL;
	else if(text == "+")		return  PLUS;
	else if (text == "=")		return ASSIGN;
	else if (text == "-")		return MINUS;
	else if (text == "*")		return MULTI;
	else if (text == ">=")		return GREATER_OR_EQUAL;
	else if (text == "<=")		return  LESS_OR_EQUAL;
	else if (text == ">")		return GREATER;
	else if (text == "<")		return LESS;
	else if (text == "/")		return  DIV;
	else if (text == "%")		return MOD;
	else if (text == "==")		return EQUAL;
	else if (text == "!=")		return NOT_EQUAL;
	else if (text == "++")		return INCREMENT;
	else if (text == "--")		return DECREMENT;
	else if(text == "{")		return LEFT_BRACKET;
	else if(text == "}")		return RIGHT_BRACKET;
	else if(text == "[")		return LEFT_SQUARE_BRACKET;
	else if(text == "]")		return RIGHT_SQUARE_BRACKET;	
	else if(text == "(")		return LEFT_PARENTHESIS;
	else if(text == ")")		return RIGHT_PARENTHESIS;
	else if(text == ";")		return SEMICOLON;	
	else if(text == ",")		return COMMA;
	else if(text == ":")		return COLON;
	else if(text == "::")		return SCOPE;
	else if(text == ".")		return DOT;
	else if(text == "..")		return DOUBLE_DOT;	
}

int yylex(YYSTYPE * yylval_param , yyscan_t yyscanner, int isEval) {
	if(isEval){
		isEval = 0;	
		return EVAL_PARSER;
	}
	else
		return yylex(yylval_param, yyscanner);
}