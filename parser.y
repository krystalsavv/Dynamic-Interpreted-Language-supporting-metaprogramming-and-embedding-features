%{
	#include <stdio.h>
	#include <string>
	#include <errno.h>
	#include <iostream>
	#include <fstream>
	#include "parser.hpp"
	#include "scanner.h"

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
%type <stringVal> elist idlist callsuffix normcall methodcall indexed indexedelem



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
		|	{printf("Program\n");}
		;

stmt : expr SEMICOLON {printf("Expresion\n");}		
	| ifstmt	{printf("IF statement\n");}
	| whilestmt	{printf("WHILE statement\n");}
	| forstmt	{printf("FOR statement\n");}
	| returnstmt	
			{	
				printf("RETURN statement\n");
			}
	| BREAK SEMICOLON	
			{
				printf("BREAK\n");
			}
	| CONTINUE SEMICOLON	
			{
				printf("CONTINUE\n");
			}
	| block 	{printf("Block\n");}
	| funcdef	{printf("Function definition\n");}
	| SEMICOLON	{printf("SEMICOLON ;\n");}
	;

expr :	 assignexpr		{}
	| expr PLUS expr	{printf("expression  + expression -> %d+%d\n",$1,$3); }
	| expr MINUS expr	{printf("expression  - expression -> %d-%d\n",$1,$3); }
	| expr MULTI expr		{printf("expression  * expression -> %d*%d\n",$1,$3); }
	| expr DIV expr		{printf("expression  / expression -> %d/%d\n",$1,$3); }
	| expr MOD expr		{printf("expression %% expression -> %d %% %d\n",$1,$3); }
	| expr GREATER expr	{printf("expression  > expression -> %d>%d\n",$1,$3); }
	| expr GREATER_OR_EQUAL expr	{printf("expression >= expression -> %d>=%d\n",$1,$3); }
	| expr LESS expr	{printf("expression  < expression -> %d<%d\n",$1,$3); }
	| expr LESS_OR_EQUAL expr	{printf("expression <= expression -> %d<=%d\n",$1,$3); }
	| expr EQUAL expr		{printf("expression == expression -> %d==%d\n",$1,$3); }
	| expr NOT_EQUAL expr	{printf("expression != expression -> %d!=%d\n",$1,$3);}
	| expr AND expr		{printf("expression && expression -> %d&&%d\n",$1,$3); }
	| expr OR expr		{printf("expression || expression -> %d/%d\n",$1,$3); }
	| term			{printf("Terminal\n");}
	| error 	{}
	;

term :	 LEFT_PARENTHESIS expr RIGHT_PARENTHESIS	{printf("( expression )\n");}
	| MINUS expr %prec UMINUS	
			{
				printf("- expression\n");
			}
	| NOT expr
			{
				printf("!expression\n");
			}
	| INCREMENT lvalue
			{
				printf("++ lvalue\n");
			}
	| lvalue INCREMENT 
			{
				printf("lvalue ++\n");
			}
	| DECREMENT lvalue
			{
				printf("-- lvalue\n");
			}
	| lvalue DECREMENT
			{
				printf("lvalue --\n");
			}
	| primary	{printf("Primary\n");}
	;

assignexpr : lvalue ASSIGN expr {printf("lvalue = expression \n");
			}
			;

primary : lvalue	{printf("lvalue\n");}
	| call	{printf("Call\n");}
	| objectdef		{printf("object definition\n");}
	| LEFT_PARENTHESIS funcdef RIGHT_PARENTHESIS	{printf("(function definition)\n");}
	| const	{printf("constant\n");}
	;

lvalue : IDENT	
			{
				printf("ID\n");
			}
	| LOCAL IDENT	
			{
				printf("Local ID\n");
			}
	| SCOPE IDENT	
			{
				printf("::ID\n");					
			}
	| member	{printf("member\n");}
	;

member : lvalue DOT IDENT {printf("lvalue dot ident\n");}
	| lvalue LEFT_SQUARE_BRACKET expr RIGHT_SQUARE_BRACKET {printf("lvalue dot ident\n");}
	| call DOT IDENT {printf("call dot ident\n");}
	| call LEFT_SQUARE_BRACKET expr RIGHT_SQUARE_BRACKET {printf("[expr]\n");}
	;

call : call LEFT_PARENTHESIS elist RIGHT_PARENTHESIS {printf("call(elist)\n");}
	| lvalue callsuffix {printf("lvalue callsufix\n");}
	| LEFT_PARENTHESIS funcdef RIGHT_PARENTHESIS LEFT_PARENTHESIS elist RIGHT_PARENTHESIS {printf("(funcdef)(elist)\n");}
	;

callsuffix : normcall {printf("normcall\n");}
	| methodcall {printf("methodcall\n");}
	;

normcall: LEFT_PARENTHESIS elist RIGHT_PARENTHESIS {printf("(elist)\n");}
		;

methodcall : DOUBLE_DOT IDENT LEFT_PARENTHESIS elist RIGHT_PARENTHESIS {printf("..ident(elist)\n");}
			;

elist :  expr {printf("exprecion\n");}
	| elist COMMA expr  {printf("elist, expresion\n");}
	|  {printf("empty (elist)\n");}
	;

objectdef : LEFT_SQUARE_BRACKET elist RIGHT_SQUARE_BRACKET 	{printf("[elist]\n");}
		| LEFT_SQUARE_BRACKET indexed RIGHT_SQUARE_BRACKET	{printf("[indexed]\n");}
		;

indexed : indexedelem {printf("indexedelem\n");}
	| indexed COMMA indexedelem	{printf("indexedelem, intexedelem\n");}
	//|   {printf("empty (indexed)\n");}
	;

indexedelem : LEFT_BRACKET expr COLON expr RIGHT_BRACKET  {printf("{ expresion : expresion }\n");
				}
			;

tmp_block: tmp_block stmt 
		| {}
		;

block : LEFT_BRACKET tmp_block RIGHT_BRACKET{ printf("{ stmt }\n");
													  }
		;

funcdef : FUNCTION IDENT 
				{
					printf("function ident");			
				}
			
			LEFT_PARENTHESIS idlist RIGHT_PARENTHESIS block
				{
					printf("function id(idlist) block\n");
				}
		| FUNCTION 
				{
				printf("function");				
				}
			LEFT_PARENTHESIS idlist RIGHT_PARENTHESIS block
				{
					printf("function (idlist) block\n");
				}
		; 

const : INTEGER {printf("Integer\n");}
		| REALNUMBER {printf("Real number\n");}
		| STRING {printf("String\n");}
		| NIL 	{printf("NIL\n");}
		| TRUE 	{printf("TRUE\n");}
		| FALSE {printf("FALSE\n");}
		;

idlist : IDENT 
		{
			printf("id list\n");							
		}
	| idlist COMMA IDENT  
		{
			printf("idlist, id\n");	
		}
	|  {printf("Empty (idlist)\n");}
	;

ifstmt : IF LEFT_PARENTHESIS expr RIGHT_PARENTHESIS stmt {printf("IF (expresion) stmt\n");}
		| ifstmt ELSE stmt {printf("ifstmt ELSE stmt\n");}
		;

whilestmt : WHILE
			{
				printf("While (expresion) stmt\n");
			}
			 LEFT_PARENTHESIS expr RIGHT_PARENTHESIS stmt 
			{
			}
			
		;

forstmt : FOR 
			{
				printf("for (elist;elist;elist) stmt\n");
			}
			LEFT_PARENTHESIS elist SEMICOLON expr SEMICOLON elist RIGHT_PARENTHESIS stmt
			{
			}
		;

returnstmt : RETURN SEMICOLON {printf("RETURN;\n");}
		| RETURN expr SEMICOLON {printf("RETURN expresion;\n");}
		;


%%

int yyerror(yyscan_t scanner, const char *yaccProvidedMessage){
	std::cout  << yaccProvidedMessage << ": at line ";// << yylineno << " before token : " << yytext << std::endl;
	std::cout << "INPUT NOT VALID \n";
	return 0;
}

// void input_pars(std::string& filename) {
// 	std::ifstream ifs;
// 	ifs.open(filename);
// 	while 


// }




int main(int argc,char** argv){
	// std::cout << "NO  PASSSS\n";
	// if(argc>1){
	// 	yyin.open(argv[1]);
	// 	if(argc==3){
	// 		yyout.open(argv[2]);
	// 	}
	// } 

	// std::cout << "PASSSS\n";
	//yyparse();

	yyscan_t scanner;
	yylex_init(&scanner);
	yyset_in(stdin, scanner);
	yyparse(scanner);

	yylex_destroy(scanner);
	return 0;
}
