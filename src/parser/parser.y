%{
	#include <stdio.h>
	#include <string>
	#include <errno.h>
	#include <iostream>	
	#include "utilities/AST.h"

	#include "parser.hpp"
	#include "scanner.h"

	using namespace interpreter; 
	int yyerror (AST* ast, yyscan_t scanner, int isEval, const char* yaccProvidedMessage);
	int yylex(YYSTYPE * yylval_param , yyscan_t yyscanner, int isEval);
%}

%code requires {
  #include "utilities/Object.h"
  #include "utilities/AST.h"
  #include "utilities/EnvironmentHolder.h"
}


%union{
	double numVal;
	Object* objectVal; 
	std::string* stringVal;
}

%error-verbose
%pure-parser
%lex-param {void * scanner}
%parse-param {AST * ast}
%parse-param {void * scanner}
%lex-param {int isEval}
%parse-param {int isEval}

%start start


%token	<stringVal>IDENT
%token  <numVal>INTEGER REALNUMBER			
%token	<stringVal>STRING

%token	MULTI_LINE_COMMENT
%token	NESTED_COMMENT

%token EVAL_PARSER

%token <stringVal> WHILE FOR IF ELSE FUNCTION RETURN BREAK CONTINUE AND OR NOT LOCAL TRUE FALSE NIL PLUS MINUS UMINUS ASSIGN MULTI MOD DIV EQUAL NOT_EQUAL INCREMENT DECREMENT 
%token <stringVal> GREATER_OR_EQUAL LESS_OR_EQUAL GREATER LESS LEFT_SQUARE_BRACKET RIGHT_SQUARE_BRACKET LEFT_BRACKET RIGHT_BRACKET LEFT_PARENTHESIS RIGHT_PARENTHESIS COMMA SEMICOLON COLON DOT DOUBLE_DOT SCOPE WRONG_DEFINITION 



%type <objectVal> stmt tmp_block
%type <objectVal> expr
%type <objectVal> ifstmt whilestmt forstmt returnstmt block funcdef funcbody
%type <objectVal> assignexpr term
%type <objectVal> lvalue primary call objectdef const member
%type <objectVal> elist idlist arg argList formal normcall methodcall indexed indexedelem
%type <objectVal> program start eval eval_stmts

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
%left LEFT_PARENTHESIS RIGHT_PARENTHESIS

%expect 1

%%
start : program 
				{
					std::cout << "Start Program" << std::endl; 
					ast->SetRoot($1);
				}
			| EVAL_PARSER  {isEval = 0;}  eval
				{
					ast->SetRoot($3);	
				}
				;

eval : expr 
			{
				$$ = $1;
			}
		| eval_stmts
			{
				$$ = $1;
			}
		|	{
				$$ = new ASTnode("type", "program");
				$$->Set("numOfStmt", 0.0);
			}
			;

eval_stmts: eval_stmts stmt 
						{
							$$ = $1;
							double numOfStmt = $$->GetValue("numOfStmt")->GetNumberValue();
							$$->Set(std::to_string((int)numOfStmt), $2);
							numOfStmt++;
							$$->Set("numOfStmt", numOfStmt);
						}
					|	stmt 
						{
							$$ = new ASTnode("type", "program");
							$$->Set(std::to_string(0), $1);
							$$->Set("numOfStmt", 1.0);
						}
						;


program : program stmt
				{ 
					std::cout << "stmt program" << std::endl;
					$$ = $1;
					double numOfStmt = $$->GetValue("numOfStmt")->GetNumberValue();
					$$->Set(std::to_string((int)numOfStmt), $2);
					numOfStmt++;
					$$->Set("numOfStmt", numOfStmt);
				}
		|	{
				std::cout << ("Program\n");
				$$ = new ASTnode("type", "program");
				$$->Set("numOfStmt", 0.0);
				//ast->SetRoot($$);
			}
		;

stmt : expr SEMICOLON	{
							std::cout << ("Expression ;\n");
							$$ = $1;

						}		
	| ifstmt			{
							std::cout << ("IF statement\n");
							$$ = $1;
						}
	| whilestmt			{
							std::cout << ("WHILE statement\n");
							$$ = $1;
						}
	| forstmt			{
							std::cout << ("FOR statement\n");						
							$$ = $1;
						}
	| returnstmt		{	
							std::cout << ("RETURN statement\n");						
							$$ = $1;
						}
	| BREAK SEMICOLON	{
							std::cout << ("BREAK\n");
							$$ = new ASTnode();
							$$->Set("type", "break");
						}
	| CONTINUE SEMICOLON	{
								std::cout << ("CONTINUE\n");
								$$ = new ASTnode();
								$$->Set("type", "continue");
							}
	| block 			{
								std::cout << ("Block\n");
								$$ = $1;
							}
	| funcdef				{
								std::cout << ("Function definition\n");
								$$ = $1;
							}
	| SEMICOLON				{	
								std::cout << ("Semicolon ;\n");
								$$ = new ASTnode();
								$$->Set("type", "semicolon");
							}
	;

expr :	 assignexpr	{ $$ = $1;}
	| expr PLUS expr	{
							std::cout << ("expression + expression \n"); 
							$$ = new ASTnode("type", "add");
							$$->Set("left", $1);
							$$->Set("right", $3);
						}
	| expr MINUS expr	{
							std::cout << ("expression - expression \n"); 
							$$ = new ASTnode("type", "sub");
							$$->Set("left", $1);
							$$->Set("right", $3);
						}
	| expr MULTI expr	{
							std::cout << ("expression * expression \n"); 
							$$ = new ASTnode("type", "mul");
							$$->Set("left", $1);
							$$->Set("right", $3);
						}
	| expr DIV expr		{
							std::cout << ("expression / expression \n"); 
							$$ = new ASTnode("type", "div");
							$$->Set("left", $1);
							$$->Set("right", $3);
						}
	| expr MOD expr		{
							std::cout << ("expression %% expression \n"); 
							$$ = new ASTnode("type", "mod");
							$$->Set("left", $1);
							$$->Set("right", $3);
						}
	| expr GREATER expr	{
							std::cout << ("expression > expression \n");
							$$ = new ASTnode("type", "greater");
							$$->Set("left", $1);
							$$->Set("right", $3);
						}
	| expr GREATER_OR_EQUAL expr	{
										std::cout << ("expression >= expression \n");
										$$ = new ASTnode("type", "greater_or_equal");
										$$->Set("left", $1);
										$$->Set("right", $3);
									}
	| expr LESS expr	{
							std::cout << ("expression  < expression \n");
							$$ = new ASTnode("type", "less");
							$$->Set("left", $1);
							$$->Set("right", $3);
						}
	| expr LESS_OR_EQUAL expr		{
										std::cout << ("expression <= expression \n" ); 
										$$ = new ASTnode("type", "less_or_equal");
										$$->Set("left", $1);
										$$->Set("right", $3);
									}
	| expr EQUAL expr	{
							std::cout << ("expression == expression \n"); 
							$$ = new ASTnode("type", "equal");
							$$->Set("left", $1);
							$$->Set("right", $3);
						}
	| expr NOT_EQUAL expr	{
								std::cout << ("expression != expression \n");
								$$ = new ASTnode("type", "not_equal");
								$$->Set("left", $1);
								$$->Set("right", $3);
							}
	| expr AND expr		{
							std::cout << ("expression && expression \n");
							$$ = new ASTnode("type", "and");
							$$->Set("left", $1);
							$$->Set("right", $3);
						}
	| expr OR expr		{
							std::cout << ("expression || expression \n");
							$$ = new ASTnode("type", "or");
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
				$$ = new ASTnode("type", "parentheses");
				$$->Set("expr", $2);
			}
	| MINUS expr %prec UMINUS	
			{
				std::cout << ("- expression\n");
				$$ = new ASTnode("type", "uminus");
				$$->Set("expr", $2);
			}
	| NOT expr
			{
				std::cout << ("!expression\n");
				$$ = new ASTnode("type", "not");
				$$->Set("expr", $2);
			}
	| INCREMENT lvalue
			{
				std::cout << ("++ lvalue\n");
				$$ = new ASTnode("type", "pre_increment");
				$$->Set("lvalue", $2);
			}
	| lvalue INCREMENT 
			{
				std::cout << ("lvalue ++\n");
				$$ = new ASTnode("type", "post_increment");
				$$->Set("lvalue", $1);
			}
	| DECREMENT lvalue
			{
				std::cout << ("-- lvalue\n");
				$$ = new ASTnode("type", "pre_decrement");
				$$->Set("lvalue", $2);
			}
	| lvalue DECREMENT
			{
				std::cout << ("lvalue --\n");
				$$ = new ASTnode("type", "post_decrement");
				$$->Set("lvalue", $1);
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
					$$ = $2;
					$$->Set("type", "parentheses_funcdef");
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
				$$->Set("ID", *$1);
				delete($1); 
			}
	| LOCAL IDENT	
			{
				std::cout << ("Local ID\n");
				$$ = new ASTnode("type", "localVar");					
				$$->Set("ID", *$2);
				delete($2); 
			}
	| SCOPE IDENT	
			{
				std::cout << ("::ID\n");
				$$ = new ASTnode("type", "globalVar");		
				$$->Set("ID", *$2);
				delete($2); 
			}
	| member	
			{ 
				std::cout << ("member\n");
				$$ = $1; 
			}
			;

member : lvalue DOT IDENT
				{
					std::cout << ("lvalue dot ident\n");
					$$ = new ASTnode("type", "member_lvalueVar");
					$$->Set("lvalue", $1);
					$$->Set("ID", *$3);
					delete($3); 
				
				}
	| lvalue LEFT_SQUARE_BRACKET expr RIGHT_SQUARE_BRACKET 
				{
					std::cout << ("lvalue dot ident\n");
					$$ = new ASTnode("type", "member_lvalueBrackets");
					$$->Set("lvalue", $1);
					$$->Set("expr", $3);
				}
	| call DOT IDENT 
				{
					std::cout << ("call dot ident\n");
					$$ = new ASTnode("type", "member_callVar");
					$$->Set("call", $1);
					$$->Set("ID", *$3);
					delete($3); 
				}
	| call LEFT_SQUARE_BRACKET expr RIGHT_SQUARE_BRACKET
				{
					std::cout << ("[expr]\n");
					$$ = new ASTnode("type", "member_callBrackets");
					$$->Set("call", $1);
					$$->Set("expr", $3);
				}
	;

call : call LEFT_PARENTHESIS argList RIGHT_PARENTHESIS 
				{
					std::cout << ("call(argList)\n");
					$$ = new ASTnode("type", "multiCall");
					$$->Set("call", $1);
					$$->Set("argList", $3);
				}
	| lvalue normcall 
				{
					std::cout << ("lvalue callsufix\n");
					$$ = new ASTnode("type", "lvalueNormCall");
					$$->Set("lvalue", $1);
					$$->Set("argList", $2);
				}
	| lvalue methodcall 
				{
					std::cout << ("lvalue callsufix\n");
					$$ = $2; 
					$$->Set("type", "lvalueMethodCall");
					$$->Set("lvalue", $1);
				}
	| LEFT_PARENTHESIS funcdef RIGHT_PARENTHESIS LEFT_PARENTHESIS argList RIGHT_PARENTHESIS
				{
					std::cout << ("(funcdef)(argList)\n");
					$$ = new ASTnode("type", "funcdefCall");
					$$->Set("funcdef", $2);
					$$->Set("argList", $5);
				}
	;


normcall: LEFT_PARENTHESIS argList RIGHT_PARENTHESIS 
			{
				std::cout << ("(argList)\n");
				$$ = $2; 
			}
			;

methodcall : DOUBLE_DOT IDENT LEFT_PARENTHESIS argList RIGHT_PARENTHESIS 
			{
				std::cout << ("..ident(argList)\n");
				$$ = new ASTnode("type", "methodcall");
				$$->Set("ID", *$2); 
				$$->Set("argList", $4); 
				
			}
			;

arg : expr	{ 
				std::cout << "expr\n";
				$$ = $1;
			} 
	| IDENT COLON expr
			{ 
				std::cout << "ID : expr\n";
				$$ = new ASTnode("type", "namedParam");
				$$->Set("expr", $3);
				$$->Set("ID", *$1);
				delete($1);
			} 
			;

argList : arg 
			{ 
				$$ = new ASTnode("type", "argList");
				$$->Set("numOfTotalArgs", 1.0);
				$$->Set("numOfPositionalArgs", 0.0);
				$$->Set("PositionalArgs", new Object());
				$$->Set("NamedArgs", new Object());
				if($1->GetValue("type")->GetStringValue() == "namedParam") {		
					$$->GetValue("NamedArgs")->GetObjectValue()->Set(*($1->GetValue("ID")), *($1->GetValue("expr")));
				}
				else {	
					$$->Set("numOfPositionalArgs", 1.0);
					$$->GetValue("PositionalArgs")->GetObjectValue()->Set(0.0, $1);
				}
			}	
	| argList COMMA arg
			{
				$$ = $1;
				double numOfTotalArgs = $$->GetValue("numOfTotalArgs")->GetNumberValue();
				if($3->GetValue("type")->GetStringValue() == "namedParam") {
					$$->GetValue("NamedArgs")->GetObjectValue()->Set(*($3->GetValue("ID")), *($3->GetValue("expr")));
				}
				else {
					double numOfPositionalArgs = $$->GetValue("numOfPositionalArgs")->GetNumberValue();
					if(numOfTotalArgs != numOfPositionalArgs) {
						throw RuntimeErrorException("Positional paramiter after named parameter"); 
					}
					$$->GetValue("PositionalArgs")->GetObjectValue()->Set(numOfPositionalArgs, $3);
					numOfPositionalArgs++;
					$$->Set("numOfPositionalArgs", numOfPositionalArgs);
				}
				numOfTotalArgs++;
				$$->Set("numOfTotalArgs", numOfTotalArgs);
			}
	|		{
				std::cout << "empty argList\n";
				$$ = new ASTnode("type", "emptyArgList");
			}
	;

elist :  expr 
			{
				std::cout << ("expression\n");
				$$ = new ASTnode("type", "elist");
				$$->Set("numOfExprs", 1.0);
				$$->Set("0", $1);
			}
	| elist COMMA expr  
			{
				std::cout << ("elist, expression\n");
				$$ = $1;
				double numOfExprs = $$->GetValue("numOfExprs")->GetNumberValue();
				$$->Set(std::to_string((int)numOfExprs), $3);
				numOfExprs++;
				$$->Set("numOfExprs", numOfExprs);
			}
	|		{
				std::cout << ("empty (elist)\n");
				$$ = new ASTnode("type", "emptyElist");
			}
	;

objectdef : LEFT_SQUARE_BRACKET elist RIGHT_SQUARE_BRACKET 
			{
				std::cout << ("[elist]\n");
				$$ = new ASTnode("type", "elistObjectdef");
				$$->Set("elist", $2);
			}
		| LEFT_SQUARE_BRACKET indexed RIGHT_SQUARE_BRACKET	
			{
				std::cout << ("[indexed]\n");
				$$ = new ASTnode("type", "indexedObjectdef");
				$$->Set("indexed", $2);
			}
			;

indexed : indexedelem
			{
				std::cout << ("indexedelem\n");
				$$ = new ASTnode("type", "indexed");
				$$->Set("numOfElems", 1.0);
				$$->Set("0", $1); 
			}
	| indexed COMMA indexedelem	
			{
				std::cout << ("indexedelem, intexedelem\n");
				$$ = $1;
				double numOfElems = $$->GetValue("numOfElems")->GetNumberValue();
				$$->Set(std::to_string((int)numOfElems), $3);
				numOfElems++;
				$$->Set("numOfElems", numOfElems);
			}
			;

indexedelem : LEFT_BRACKET expr COLON expr RIGHT_BRACKET  
				{
					std::cout << ("{ expression : expression }\n");
					$$ = new ASTnode("type", "indexedElem");
					$$->Set("keyExpr", $2);
					$$->Set("valueExpr", $4);
				}
				;

tmp_block: tmp_block stmt
				{
					$$ = $1;
					double numOfStmt = $$->GetValue("numOfStmt")->GetNumberValue();
					$$->Set(std::to_string((int)numOfStmt), $2);
					numOfStmt++;
					$$->Set("numOfStmt", numOfStmt);
				}
		|		{ 
					$$ = new ASTnode("type", "block");
					$$->Set("numOfStmt", 0.0);
				}
		;

block : LEFT_BRACKET tmp_block RIGHT_BRACKET
			{
				std::cout << ("{ stmt }\n");
				$$ = $2; 
			}
		;


funcbody: LEFT_BRACKET tmp_block RIGHT_BRACKET
				{
					std::cout << ("{ stmt }\n");
					$$ = $2; 
					$2->Set("type", "funcbody");
				}
		;

funcdef : FUNCTION IDENT 
				{
					std::cout << ("function ident\n");
				}
			
			LEFT_PARENTHESIS idlist RIGHT_PARENTHESIS funcbody
				{
					std::cout << ("function id(idlist) funcbody\n");
					$$ = new ASTnode("type", "funcdef");
					$$->Set("ID", *$2);
					ASTnode* funcEnter = new ASTnode("type", "funcEnter");
					funcEnter->Set("idlist", $5);			//prosoxh an vgalw panw action
					funcEnter->Set("funcbody", $7);
					$$->Set("funcEnter", funcEnter);
				}
		| FUNCTION 
				{
					std::cout << ("function\n");				
				}
			LEFT_PARENTHESIS idlist RIGHT_PARENTHESIS funcbody
				{
					std::cout << ("function (idlist) funcbody\n");
					$$ = new ASTnode("type", "anonymousFuncdef");
					ASTnode* funcEnter = new ASTnode("type", "funcEnter");
					funcEnter->Set("idlist", $4);			//prosoxh an vgalw panq action	
					funcEnter->Set("funcbody", $6);
					$$->Set("funcEnter", funcEnter);
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
					$$->Set("value", *$1);
					delete($1);
				}
		| TRUE 	{
					std::cout << ("TRUE\n");
					$$ = new ASTnode("type", "boolConst");
					$$->Set("value", true);
				}
		| FALSE {
					std::cout << ("FALSE\n");
					$$ = new ASTnode("type", "boolConst");
					$$->Set("value", false);
				}
		| NIL 	{
					std::cout << ("NIL\n");
					$$ = new ASTnode("type", "nil");
				}
		;

formal: IDENT 
		{
			std::cout << ("id list\n");
			$$ = new ASTnode("type", "param");
			$$->Set("ID", *$1);
		}
	
	| IDENT ASSIGN expr
		{
			std::cout << ("id =  expr\n");
			$$ = new ASTnode("type", "optionalParam");
			$$->Set("ID", *$1);
			$$->Set("expr", $3);
		}
	;


idlist : formal 
		{
			std::cout << ("formal\n");
			$$ = new ASTnode("type", "idlist");
			$$->Set("numOfParams", 1.0);
			$$->Set(0.0, $1); 
			}
	| idlist COMMA formal	
			{
				std::cout << ("idlist, id\n");	
				$$ = $1;
				double numOfParams = $$->GetValue("numOfParams")->GetNumberValue();
				$$->Set(numOfParams, $3);
				numOfParams++;
				$$->Set("numOfParams", numOfParams);
			}
	|		{
				std::cout << ("Empty (idlist)\n");
				$$ = new ASTnode("type", "emptyIdlist");
				$$->Set("numOfParams", 0.0);
			}
			;

ifstmt : IF LEFT_PARENTHESIS expr RIGHT_PARENTHESIS stmt 
			{
				std::cout << ("IF (expression) stmt\n");
				$$ = new ASTnode("type", "ifstmt");
				$$->Set("condition", $3);					
				$$->Set("stmt", $5);

			}
		| ifstmt ELSE stmt 
			{
				std::cout << ("ifstmt ELSE stmt\n");
				$$ = new ASTnode("type", "if_elsestmt");
				$$->Set("ifstmt", $1);					
				$$->Set("elsestmt", $3);
			}
			;

whilestmt : WHILE
			{
				std::cout << ("While (expression) stmt\n");
			}
			 LEFT_PARENTHESIS expr RIGHT_PARENTHESIS stmt 
			{
				$$ = new ASTnode("type", "whilestmt");
				$$->Set("condition", $4);						// SOS: change if the action removed!!!
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
				$$->Set("init_elist", $4);						// SOS: change if the action removed!!!
				$$->Set("condition", $6);
				$$->Set("elist", $8);
				$$->Set("stmt", $10);	
			}
			;

returnstmt : RETURN SEMICOLON 
				{
					std::cout << ("RETURN;\n");
					$$ = new ASTnode("type", "return");
				}
		| RETURN expr SEMICOLON 
				{
					std::cout << ("RETURN expression;\n");
					$$ = new ASTnode("type", "return_value");
					$$->Set("expr", $2);
				}
				;


%%

int yyerror(AST* ast, yyscan_t scanner, int isEval, const char *yaccProvidedMessage){
	std::string yaccProvidedMessage_string = yaccProvidedMessage;
	throw SyntaxErrorException(yaccProvidedMessage_string + " undefined token  \" "  + yyget_text(scanner) + "\"");
	return 0;
}