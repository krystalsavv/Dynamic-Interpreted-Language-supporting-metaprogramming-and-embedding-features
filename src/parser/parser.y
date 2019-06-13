%{
	#include <stdio.h>
	#include <string>
	#include <errno.h>
	#include <iostream>	
	#include "utilities/AST.h"


	#include "parser.hpp"
	#include "scanner.h"

	int yyerror (AST* root, yyscan_t scanner, const char* yaccProvidedMessage);
%}

%code requires {
  #include "utilities/Object.h"
  #include "utilities/AST.h"
}


%union{
	//int intVal;
	double numVal;
	Object* objectVal; 
	std::string* stringVal;
}

%error-verbose
%pure-parser
%lex-param {void * scanner}
%parse-param {AST * root}
%parse-param {void * scanner}

%start program

%token	<stringVal>IDENT
%token  <numVal>INTEGER REALNUMBER			
%token	<stringVal>STRING

%token	MULTI_LINE_COMMENT
%token	NESTED_COMMENT

%token <stringVal> WHILE FOR IF ELSE FUNCTION RETURN BREAK CONTINUE AND OR NOT LOCAL TRUE FALSE NIL PLUS MINUS UMINUS ASSIGN MULTI MOD DIV EQUAL NOT_EQUAL INCREMENT DECREMENT 
%token <stringVal> GREATER_OR_EQUAL LESS_OR_EQUAL GREATER LESS LEFT_SQUARE_BRACKET RIGHT_SQUARE_BRACKET LEFT_BRACKET RIGHT_BRACKET LEFT_PARENTHESIS RIGHT_PARENTHESIS COMMA SEMICOLON COLON DOT DOUBLE_DOT SCOPE WRONG_DEFINITION 



%type <objectVal> stmt tmp_block
%type <objectVal> expr
%type <objectVal> ifstmt whilestmt forstmt returnstmt block funcdef
%type <objectVal> assignexpr term
%type <objectVal> lvalue primary call objectdef const member
%type <objectVal> elist idlist arg argList formal callsuffix normcall methodcall indexed indexedelem
%type <objectVal> program

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
program : stmt program { std::cout << "stmt program    " << std::endl; }
		|	{std::cout << ("Program\n");}
		;

stmt : expr SEMICOLON	{
							std::cout << ("Expression ;\n");
							//$$ = new ASTnode();
							//$$->Set("type", "stmt");
							//$$->Set("expr", $1);
							$$ = $1;

						}		
	| ifstmt			{
							std::cout << ("IF statement\n");
							//$$ = new ASTnode();
							//$$->Set("type", "stmt");
							//$$->Set("ifstmt", $1);
							$$ = $1;
						}
	| whilestmt			{
							std::cout << ("WHILE statement\n");
							//$$ = new ASTnode();
							//$$->Set("type", "stmt");
							//$$->Set("whilestmt", $1);
							$$ = $1;
						}
	| forstmt			{
							std::cout << ("FOR statement\n");
							//$$ = new ASTnode();
							//$$->Set("type", "stmt");			
							//$$->Set("forstmt", $1);
							$$ = $1;
						}
	| returnstmt		{	
							std::cout << ("RETURN statement\n");
							//$$ = new ASTnode();
							//$$->Set("type", "stmt");
							//$$->Set("returnstmt", $1);
							$$ = $1;
						}
	| BREAK SEMICOLON	{
							std::cout << ("BREAK\n");
							$$ = new ASTnode();
							$$->Set("type", "BREAK");
						}
	| CONTINUE SEMICOLON	{
								std::cout << ("CONTINUE\n");
								$$ = new ASTnode();
								$$->Set("type", "CONTINUE");
							}
	| block 				{
								std::cout << ("Block\n");
								//$$ = new ASTnode();
								//$$->Set("type", "stmt");
								//$$->Set("block", $1);
								$$ = $1;
							}
	| funcdef				{
								std::cout << ("Function definition\n");
								//$$ = new ASTnode();
								//$$->Set("type", "stmt");
								//$$->Set("funcdef", $1);
								$$ = $1;
							}
	| SEMICOLON				{	
								std::cout << ("Semicolon ;\n");
								$$ = new ASTnode();
								$$->Set("type", "SEMICOLON");
							}
	;

expr :	 assignexpr		{ $$ = $1; }
	| expr PLUS expr	{
							std::cout << ("expression + expression \n"); 
							$$ = new ASTnode("type", "PLUS");
							$$->Set("left", $1);
							$$->Set("right", $3);
						}
	| expr MINUS expr	{
							std::cout << ("expression - expression \n"); 
							$$ = new ASTnode("type", "MINUS");
							$$->Set("left", $1);
							$$->Set("right", $3);
						}
	| expr MULTI expr	{
							std::cout << ("expression * expression \n"); 
							$$ = new ASTnode("type", "MULTI");
							$$->Set("left", $1);
							$$->Set("right", $3);
						}
	| expr DIV expr		{
							std::cout << ("expression / expression \n"); 
							$$ = new ASTnode("type", "DIV");
							$$->Set("left", $1);
							$$->Set("right", $3);
						}
	| expr MOD expr		{
							std::cout << ("expression %% expression \n"); 
							$$ = new ASTnode("type", "MOD");
							$$->Set("left", $1);
							$$->Set("right", $3);
						}
	| expr GREATER expr	{
							std::cout << ("expression > expression \n");
							$$ = new ASTnode("type", "GREATER");
							$$->Set("left", $1);
							$$->Set("right", $3);
						}
	| expr GREATER_OR_EQUAL expr	{
										std::cout << ("expression >= expression \n");
										$$ = new ASTnode("type", "GREATER_OR_EQUAL");
										$$->Set("left", $1);
										$$->Set("right", $3);
									}
	| expr LESS expr	{
							std::cout << ("expression  < expression \n");
							$$ = new ASTnode("type", "LESS");
							$$->Set("left", $1);
							$$->Set("right", $3);
						}
	| expr LESS_OR_EQUAL expr		{
										std::cout << ("expression <= expression \n" ); 
										$$ = new ASTnode("type", "LESS_OR_EQUAL");
										$$->Set("left", $1);
										$$->Set("right", $3);
									}
	| expr EQUAL expr	{
							std::cout << ("expression == expression \n"); 
							$$ = new ASTnode("type", "EQUAL");
							$$->Set("left", $1);
							$$->Set("right", $3);
						}
	| expr NOT_EQUAL expr	{
								std::cout << ("expression != expression \n");
								$$ = new ASTnode("type", "NOT_EQUAL");
								$$->Set("left", $1);
								$$->Set("right", $3);
							}
	| expr AND expr		{
							std::cout << ("expression && expression \n");
							$$ = new ASTnode("type", "AND");
							$$->Set("left", $1);
							$$->Set("right", $3);
						}
	| expr OR expr		{
							std::cout << ("expression || expression \n");
							$$ = new ASTnode("type", "OR");
							$$->Set("left", $1);
							$$->Set("right", $3);
						}
	| term				{	
							std::cout << ("Terminal\n");
							$$ = $1;
						}
	| error 	{}
	;

term :	 LEFT_PARENTHESIS expr RIGHT_PARENTHESIS	
			{
				std::cout << ("( expression )\n");
				$$ = new ASTnode("type", "PARENTHESES");
				$$->Set("expr", $2);
			}
	| MINUS expr %prec UMINUS	
			{
				std::cout << ("- expression\n");
				$$ = new ASTnode("type", "UMINUS");
				$$->Set("expr", $2);
			}
	| NOT expr
			{
				std::cout << ("!expression\n");
				$$ = new ASTnode("type", "NOT");
				$$->Set("expr", $2);
			}
	| INCREMENT lvalue
			{
				std::cout << ("++ lvalue\n");
				$$ = new ASTnode("type", "INCREMENT");
				$$->Set("lvalue_after", $2);
			}
	| lvalue INCREMENT 
			{
				std::cout << ("lvalue ++\n");
				$$ = new ASTnode("type", "INCREMENT");
				$$->Set("lvalue_before", $1);
			}
	| DECREMENT lvalue
			{
				std::cout << ("-- lvalue\n");
				$$ = new ASTnode("type", "DECREMENT");
				$$->Set("lvalue_after", $2);
			}
	| lvalue DECREMENT
			{
				std::cout << ("lvalue --\n");
				$$ = new ASTnode("type", "DECREMENT");
				$$->Set("lvalue_before", $1);
			}
	| primary	{	
					std::cout << ("Primary\n");
					$$ = $1;
				}
	;

assignexpr : lvalue ASSIGN expr 
				{
					std::cout << ("lvalue = expression \n");
					$$ = new ASTnode("type", "assignexpr");
					$$->Set("lvalue", $1);
					$$->Set("expr", $3);
				}
				;

primary : lvalue	
				{
					std::cout << ("lvalue\n");
					$$ = $1;
				}
	| call		{
					std::cout << ("Call\n");
					$$ = $1;
				}
	| objectdef	{	
					std::cout << ("object definition\n");
					$$ = $1;
				}
	| LEFT_PARENTHESIS funcdef RIGHT_PARENTHESIS	
				{ 
					std::cout << ("(function definition)\n");
					$$ = new ASTnode("type", "PARENTHESES");
					$$->Set("funcdef", $2);
				}
	| const	{
				std::cout << ("constant\n");
				$$ = $1;
			}
	;

lvalue : IDENT	
			{
				std::cout << ("ID\n");
				$$ = new ASTnode("type", "var");
				$$->Set("ID", $1);
			}
	| LOCAL IDENT	
			{
				std::cout << ("Local ID\n");
				$$ = new ASTnode("type", "localVar");					// den einai sigoyrh oti xreiazetai na ta diaxwrisw
				$$->Set("ID", $2);
			}
	| SCOPE IDENT	
			{
				std::cout << ("::ID\n");
				$$ = new ASTnode("type", "scopeVar");					//   ::?? --> global?
				$$->Set("ID", $2);
			}
	| member	{ 
					std::cout << ("member\n");
					//$$ = $1; 
					$$ = new ASTnode("type", "member");					// gia na exw ola ta members katw apo ena node(type) (alliws den xreiazetai kai kanw thn panw grammh) 
					$$->Set("value", $1);
				}
	;

member : lvalue DOT IDENT
				{
					std::cout << ("lvalue dot ident\n");
					$$ = new ASTnode("type", "DOT");
					$$->Set("lvalue", $1);
					ASTnode* varNode = new ASTnode("type", "memberVar");					// den einai sigoyrh oti xreiazetai na ta diaxwrisw
					varNode->Set("ID", $3);
					$$->Set("memberVar", varNode);						//???
				
				}
	| lvalue LEFT_SQUARE_BRACKET expr RIGHT_SQUARE_BRACKET 
				{
					std::cout << ("lvalue dot ident\n");
					$$ = new ASTnode("type", "SQUARE_BRACKETS");
					$$->Set("lvalue", $1);
					$$->Set("expr", $3);
				}
	| call DOT IDENT 
				{
					std::cout << ("call dot ident\n");
					$$ = new ASTnode("type", "DOT");
					$$->Set("call", $1);
					ASTnode* varNode = new ASTnode("type", "memberVar");					// den einai sigoyrh oti xreiazetai na ta diaxwrisw
					varNode->Set("ID", $3);
					$$->Set("memberVar", varNode);
				}
	| call LEFT_SQUARE_BRACKET expr RIGHT_SQUARE_BRACKET
				{
					std::cout << ("[expr]\n");
					$$ = new ASTnode("type", "SQUARE_BRACKETS");
					$$->Set("call", $1);
					$$->Set("expr", $3);
				}
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

const : INTEGER {
					std::cout << ("Integer\n");
					$$ = new ASTnode("type", "numConst");
					$$->Set("value", $1);
				}
		| REALNUMBER {
						std::cout << ("Real number\n");
						$$ = new ASTnode("type", "numConst");
						$$->Set("value", $1);
					}
		| STRING {
					std::cout << ("String\n");
					$$ = new ASTnode("type", "stringConst");
					$$->Set("value", $1);
				}
		| NIL 	{
					std::cout << ("NIL\n");
					$$ = new ASTnode("type", "NIL");
				}
		| TRUE 	{
					std::cout << ("TRUE\n");
					$$ = new ASTnode("type", "boolConst");
					$$->Set("value", $1);
				}
		| FALSE {
					std::cout << ("FALSE\n");
					$$ = new ASTnode("type", "boolConst");
					$$->Set("value", $1);
				}
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

ifstmt : IF LEFT_PARENTHESIS expr RIGHT_PARENTHESIS stmt 
			{
				std::cout << ("IF (expression) stmt\n");
			}
		| ifstmt ELSE stmt 
			{
				std::cout << ("ifstmt ELSE stmt\n");
			}
			;

whilestmt : WHILE
			{
				std::cout << ("While (expression) stmt\n");
			}
			 LEFT_PARENTHESIS expr RIGHT_PARENTHESIS stmt 
			{
				$$ = new ASTnode("type", "whilestmt");
				$$->Set("expr", $4);						// SOS: change if the action removed!!!
				$$->Set("stmt", $6);	
			}
			;

forstmt : FOR 
			{
				std::cout << ("for (elist;elist;elist) stmt\n");
			}
			LEFT_PARENTHESIS elist SEMICOLON expr SEMICOLON elist RIGHT_PARENTHESIS stmt
			{
				$$ = new ASTnode("type", "forstmt");
				$$->Set("elist1", $4);						// SOS: change if the action removed!!!
				$$->Set("expr", $6);
				$$->Set("elist2", $8);	
			}
			;

returnstmt : RETURN SEMICOLON 
				{
					std::cout << ("RETURN;\n");
					$$ = new ASTnode("type", "RETURN");
				}
		| RETURN expr SEMICOLON 
				{
					std::cout << ("RETURN expression;\n");
					$$ = new ASTnode("type", "RETURN");
					$$->Set("expr", $2);
				}
		;


%%

int yyerror(AST* root, yyscan_t scanner, const char *yaccProvidedMessage){
	std::cout  << yaccProvidedMessage << ": at line " << yyget_lineno(scanner) << " before token : " << yyget_text(scanner) << std::endl;
	std::cout << "INPUT NOT VALID \n";
	return 0;
}