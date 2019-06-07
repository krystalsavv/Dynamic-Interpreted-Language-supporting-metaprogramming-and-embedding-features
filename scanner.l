%{
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
%option header-file="scanner.h"
%option bison-bridge

keyword 	(function)|(while)|(for)|(if)|(else)|(continue)|(break)|(return)|(and)|(or)|(not)|(local)|(true)|(false)|(nil)
operator 	[\*\/\%]|(\={1,2})|(\>\=?)|(\<\=?)|(\!\=)|(\+{1,2})|(\-{1,2})
integer 	(0[xX][0-9A-Fa-f]+)|([0-9]+)
real 		[0-9]+\.[0-9](e|E)?[0-9]*	
space 		[\r\n \t\v]
id 			[a-zA-Z][a-zA-Z_0-9]* 
comment1 	"//".*
comment2 	"#".*
punc 		[\[\]\{\}\(\)\;\,]|(\:{1,2})|(\.{1,2})

%x STRING_
%x COMMENT3

%%

"\"" 						{
								stringVal = "";
								BEGIN(STRING_);
							}

<STRING_>"\""				{
								yylval->stringVal = &stringVal; 
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


<STRING_>"\\"				{	   
							 	std::cerr << "WARNING: invalid escape character "<< yytext << " in line "<< yylineno << " will be ignored.\n";  
							}
<STRING_><<EOF>> 			{ std::cerr << "ERROR: not closed string at line " << yylineno << std::endl; yyterminate(); } 	

<STRING_>.					{ stringVal += yytext; }


"/*" 						{ BEGIN(COMMENT3); }

<COMMENT3>"/*" 				{ commentCounter++; }

<COMMENT3>"*/" 				{
								commentCounter--;
								if(!commentCounter)
									BEGIN(INITIAL);
							}

<COMMENT3><<EOF>>  			{ std::cerr << "ERROR: not closed comment at line " << yylineno << std::endl; yyterminate(); } 	

<COMMENT3>. 				{}

"*/" 						{ std::cerr << "ERROR: not closed comment at line " << yylineno << std::endl; yyterminate(); } 	


{keyword}	{return keywords_to_enum(yytext);}
{operator}	{return keywords_to_enum(yytext);}
{integer} 	{yylval->intVal = atoi(yytext);
					return INTEGER;}
{real} 		{yylval->doubleVal = atof(yytext);
return REALNUMBER;}
{space}		{}
{comment1}	{}
{comment2}	{}
{punc}		{return keywords_to_enum(yytext);}
{id}		{	stringVal.assign(yytext); 
				yylval->stringVal = &stringVal; 
				return IDENT; }

. 			{std::cerr << "Line " << yylineno << " : Lexical analysis error:You can not have an identifier starting with number or symbol $ (WRONG DEFINITION). \n"; return WRONG_DEFINITION;}

%%

int keywords_to_enum(std::string text){
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