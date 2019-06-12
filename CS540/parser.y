%{
	#include <stdio.h>
	#include <string>
	#include <errno.h>
	#include <iostream>
	#include <fstream>
	#include "parser.hpp"
	#include "scanner.h"
	#include "Object.h"

	int yyerror (yyscan_t scanner, const char* yaccProvidedMessage);
%}

%union{
	std::string* stringVal=nullptr;
	int intVal;
	double doubleVal;
}
%error-verbose


%pure-parser
%lex-param {void * scanner}
%parse-param {void * scanner}

%start program

%token	<stringVal>IDENT
%token  <intVal>INTEGER 			
%token	<doubleVal> REALNUMBER
%token	<stringVal>STRING

%token	MULTI_LINE_COMMENT
%token	NESTED_COMMENT

%token <stringVal> WHILE FOR IF ELSE FUNCTION RETURN BREAK CONTINUE AND OR NOT LOCAL TRUE FALSE NIL PLUS MINUS UMINUS ASSIGN MULTI MOD DIV EQUAL NOT_EQUAL INCREMENT DECREMENT 
%token <stringVal> GREATER_OR_EQUAL LESS_OR_EQUAL GREATER LESS LEFT_SQUARE_BRACKET RIGHT_SQUARE_BRACKET LEFT_BRACKET RIGHT_BRACKET LEFT_PARENTHESIS RIGHT_PARENTHESIS COMMA SEMICOLON COLON DOT DOUBLE_DOT SCOPE WRONG_DEFINITION 



%type <stringVal> stmt tmp_block
%type <intVal> expr
%type <stringVal> ifstmt whilestmt forstmt returnstmt block funcdef
%type <stringVal> assignexpr term
%type <stringVal> lvalue primary call objectdef const member
%type <stringVal> elist idlist arg argList formal callsuffix normcall methodcall indexed indexedelem


%right ASSIGN
%left OR
%left AND
%nonassoc EQUAL NOT_EQUAL
%nonassoc GREATER_OR_EQUAL LESS_OR_EQUAL GREATER LESS
%left PLUS MINUS
%left MULTI DIV MOD
%right NOT INCREMENT DECREMENT UMINUS
%left DOT DOUBLE_DOT
%left LEFT_SQUARE_BRACKET RIGHT_SQUARE_BRACKET
%left LEFT_PARENTHESIS	RIGHT_PARENTHESIS

%expect 1

%%
program : stmt program 
		|	{std::cout << ("Program\n");}
		;

stmt : expr SEMICOLON {std::cout << ("Expression ;\n");}		
	| ifstmt	{std::cout << ("IF statement\n");}
	| whilestmt	{std::cout << ("WHILE statement\n");}
	| forstmt	{std::cout << ("FOR statement\n");}
	| returnstmt	
			{	
				std::cout << ("RETURN statement\n");
			}
	| BREAK SEMICOLON	
			{
				std::cout << ("BREAK\n");
			}
	| CONTINUE SEMICOLON	
			{
				std::cout << ("CONTINUE\n");
			}
	| block 	{std::cout << ("Block\n");}
	| funcdef	{std::cout << ("Function definition\n");}
	| SEMICOLON	{std::cout << ("Semicolon ;\n");}
	;

expr :	 assignexpr		{}
	| expr PLUS expr	{std::cout << ("expression + expression \n"); }
	| expr MINUS expr	{std::cout << ("expression - expression \n"); }
	| expr MULTI expr		{std::cout << ("expression * expression \n"); }
	| expr DIV expr		{std::cout << ("expression / expression \n"); }
	| expr MOD expr		{std::cout << ("expression %% expression \n"); }
	| expr GREATER expr	{std::cout << ("expression > expression \n"); }
	| expr GREATER_OR_EQUAL expr	{std::cout << ("expression >= expression \n"); }
	| expr LESS expr	{std::cout << ("expression  < expression \n"); }
	| expr LESS_OR_EQUAL expr	{std::cout << ("expression <= expression \n" ); }
	| expr EQUAL expr		{std::cout << ("expression == expression \n"); }
	| expr NOT_EQUAL expr	{std::cout << ("expression != expression \n");}
	| expr AND expr		{std::cout << ("expression && expression \n"); }
	| expr OR expr		{std::cout << ("expression || expression \n"); }
	| term			{std::cout << ("Terminal\n");}
	| error 	{}
	;

term :	 LEFT_PARENTHESIS expr RIGHT_PARENTHESIS	{std::cout << ("( expression )\n");}
	| MINUS expr %prec UMINUS	
			{
				std::cout << ("- expression\n");
			}
	| NOT expr
			{
				std::cout << ("!expression\n");
			}
	| INCREMENT lvalue
			{
				std::cout << ("++ lvalue\n");
			}
	| lvalue INCREMENT 
			{
				std::cout << ("lvalue ++\n");
			}
	| DECREMENT lvalue
			{
				std::cout << ("-- lvalue\n");
			}
	| lvalue DECREMENT
			{
				std::cout << ("lvalue --\n");
			}
	| primary	{std::cout << ("Primary\n");}
	;

assignexpr : lvalue ASSIGN expr {std::cout << ("lvalue = expression \n");
			}
			;

primary : lvalue	{std::cout << ("lvalue\n");}
	| call	{std::cout << ("Call\n");}
	| objectdef		{std::cout << ("object definition\n");}
	| LEFT_PARENTHESIS funcdef RIGHT_PARENTHESIS	{std::cout << ("(function definition)\n");}
	| const	{std::cout << ("constant\n");}
	;

lvalue : IDENT	
			{
				std::cout << ("ID\n");
			}
	| LOCAL IDENT	
			{
				std::cout << ("Local ID\n");
			}
	| SCOPE IDENT	
			{
				std::cout << ("::ID\n");					
			}
	| member	{std::cout << ("member\n");}
	;

member : lvalue DOT IDENT {std::cout << ("lvalue dot ident\n");}
	| lvalue LEFT_SQUARE_BRACKET expr RIGHT_SQUARE_BRACKET {std::cout << ("lvalue dot ident\n");}
	| call DOT IDENT {std::cout << ("call dot ident\n");}
	| call LEFT_SQUARE_BRACKET expr RIGHT_SQUARE_BRACKET {std::cout << ("[expr]\n");}
	;

call : call LEFT_PARENTHESIS argList RIGHT_PARENTHESIS {std::cout << ("call(argList)\n");}
	| lvalue callsuffix {std::cout << ("lvalue callsufix\n");}
	| LEFT_PARENTHESIS funcdef RIGHT_PARENTHESIS LEFT_PARENTHESIS argList RIGHT_PARENTHESIS {std::cout << ("(funcdef)(argList)\n");}
	;

callsuffix : normcall {std::cout << ("normcall\n");}
	| methodcall {std::cout << ("methodcall\n");}
	;

normcall: LEFT_PARENTHESIS argList RIGHT_PARENTHESIS {std::cout << ("(argList)\n");}
		;

methodcall : DOUBLE_DOT IDENT LEFT_PARENTHESIS argList RIGHT_PARENTHESIS {std::cout << ("..ident(argList)\n");}
			;

arg : expr { std::cout << "expr\n"; } 
	| IDENT COLON expr { std::cout << "ID : expr\n"; } 
	;

argList : arg { std::cout << "arg\n"; }	
	| argList COMMA arg { std::cout << "argList, arg\n"; }
	| { std::cout << "empty argList\n"; }
	;

elist :  expr {std::cout << ("expression\n");}
	| elist COMMA expr  {std::cout << ("elist, expression\n");}
	|  {std::cout << ("empty (elist)\n");}
	;

objectdef : LEFT_SQUARE_BRACKET elist RIGHT_SQUARE_BRACKET 	{std::cout << ("[elist]\n");}
		| LEFT_SQUARE_BRACKET indexed RIGHT_SQUARE_BRACKET	{std::cout << ("[indexed]\n");}
		;

indexed : indexedelem {std::cout << ("indexedelem\n");}
	| indexed COMMA indexedelem	{std::cout << ("indexedelem, intexedelem\n");}
	//|   {std::cout << ("empty (indexed)\n");}
	;

indexedelem : LEFT_BRACKET expr COLON expr RIGHT_BRACKET  {std::cout << ("{ expression : expression }\n");
				}
			;

tmp_block: tmp_block stmt 
		| {}
		;

block : LEFT_BRACKET tmp_block RIGHT_BRACKET{ std::cout << ("{ stmt }\n");
													  }
		;

funcdef : FUNCTION IDENT 
				{
					std::cout << ("function ident\n");			
				}
			
			LEFT_PARENTHESIS idlist RIGHT_PARENTHESIS block
				{
					std::cout << ("function id(idlist) block\n");
				}
		| FUNCTION 
				{
				std::cout << ("function\n");				
				}
			LEFT_PARENTHESIS idlist RIGHT_PARENTHESIS block
				{
					std::cout << ("function (idlist) block\n");
				}
		; 

const : INTEGER {std::cout << ("Integer\n");}
		| REALNUMBER {std::cout << ("Real number\n");}
		| STRING {std::cout << ("String\n");}
		| NIL 	{std::cout << ("NIL\n");}
		| TRUE 	{std::cout << ("TRUE\n");}
		| FALSE {std::cout << ("FALSE\n");}
		;

formal: IDENT 
		{
			std::cout << ("id list\n");
		}
	
	| IDENT ASSIGN expr
		{
			std::cout << ("id =  expr\n");
		}
	;


idlist : formal 
		{
			std::cout << ("formal\n");							
		}
	| idlist COMMA formal  
		{
			std::cout << ("idlist, id\n");	
		}
	|  {std::cout << ("Empty (idlist)\n");}
	;

ifstmt : IF LEFT_PARENTHESIS expr RIGHT_PARENTHESIS stmt {std::cout << ("IF (expression) stmt\n");}
		| ifstmt ELSE stmt {std::cout << ("ifstmt ELSE stmt\n");}
		;

whilestmt : WHILE
			{
				std::cout << ("While (expression) stmt\n");
			}
			 LEFT_PARENTHESIS expr RIGHT_PARENTHESIS stmt 
			{
			}
			
		;

forstmt : FOR 
			{
				std::cout << ("for (elist;elist;elist) stmt\n");
			}
			LEFT_PARENTHESIS elist SEMICOLON expr SEMICOLON elist RIGHT_PARENTHESIS stmt
			{
			}
		;

returnstmt : RETURN SEMICOLON {std::cout << ("RETURN;\n");}
		| RETURN expr SEMICOLON {std::cout << ("RETURN expression;\n");}
		;


%%

int yyerror(yyscan_t scanner, const char *yaccProvidedMessage){
	std::cout  << yaccProvidedMessage << ": at line " << yyget_lineno(scanner) << " before token : " << yyget_text(scanner) << std::endl;
	std::cout << "INPUT NOT VALID \n";
	return 0;
}


std::string InputToString(const char * filename) {
	std::ifstream ifs(filename);
	std::string str((std::istreambuf_iterator<char>(ifs)), std::istreambuf_iterator<char>());

	return str;
}


int main(int argc,char** argv){
	yyscan_t scanner;
	yylex_init(&scanner);

	if(argc>1) {
		std::string inputString = InputToString(argv[1]);
		yy_scan_string(inputString.c_str(), scanner);
		// if(argc==3){
		//	FILE * output = fopen(argv[2], "w");
	 	// 	yyset_out(output, scanner);
	 	// }
	} else {
		yyset_in(stdin, scanner);
	}
	yyparse(scanner);
	yylex_destroy(scanner);

	Object* obj = new Object("kato",5);
	Object* obj1 = new Object("obj",obj);
	obj1->PrintMap();

	// stay window open VS
	//int x;
	//std::cin >> x;


	return 0;
}