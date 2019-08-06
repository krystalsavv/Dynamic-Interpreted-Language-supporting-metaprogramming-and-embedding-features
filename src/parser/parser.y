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
  extern size_t  interpreter::lineNumber;
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

%token EVAL_PARSER META_PARSER

%token WHILE FOR IF ELSE FUNCTION RETURN BREAK CONTINUE AND OR NOT LOCAL TRUE FALSE NIL PLUS MINUS UMINUS ASSIGN MULTI MOD DIV EQUAL NOT_EQUAL INCREMENT DECREMENT 
%token GREATER_OR_EQUAL LESS_OR_EQUAL GREATER LESS LEFT_SQUARE_BRACKET RIGHT_SQUARE_BRACKET LEFT_BRACKET RIGHT_BRACKET LEFT_PARENTHESIS RIGHT_PARENTHESIS COMMA SEMICOLON COLON DOT DOUBLE_DOT SCOPE WRONG_DEFINITION 

%token	META_SYNTAX_OPEN META_SYNTAX_CLOSE META_ESCAPE META_EXECUTE META_PARSE META_UNPARSE 


%type <objectVal> stmt tmp_block
%type <objectVal> expr
%type <objectVal> ifstmt whilestmt forstmt returnstmt block funcdef funcbody
%type <objectVal> assignexpr term
%type <objectVal> lvalue primary call objectdef const member
%type <objectVal> elist idlist arg argList formal normcall methodcall indexed indexedelem
%type <objectVal> program start eval eval_stmts
%type <objectVal> meta meta_syntax meta_escape meta_execute meta_parse meta_unparse

%right ASSIGN
%left OR
%left AND
%nonassoc EQUAL NOT_EQUAL
%nonassoc GREATER_OR_EQUAL LESS_OR_EQUAL GREATER LESS
%left PLUS MINUS
%left MULTI DIV MOD
%right NOT INCREMENT DECREMENT UMINUS
%left DOT DOUBLE_DOT
%right META_UNPARSE META_PARSE 
%right META_EXECUTE
%right META_SYNTAX_OPEN META_SYNTAX_CLOSE
%right META_ESCAPE
%left LEFT_SQUARE_BRACKET RIGHT_SQUARE_BRACKET
%left LEFT_PARENTHESIS RIGHT_PARENTHESIS

%expect 1

%%
start : program {
					ast->SetRoot($1);
				}
			| EVAL_PARSER  {isEval = 0;}  eval
				{
					ast->SetRoot($3);	
				}
			| META_PARSER {isEval = 0;} expr
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
				$$->SetLine(yyget_lineno(scanner));
			}
			;

eval_stmts: eval_stmts stmt 
						{
							$$ = $1;
							double numOfStmt = $$->GetValue("numOfStmt")->GetNumberValue();
							$$->Set(numOfStmt, $2);
							numOfStmt++;
							$$->Set("numOfStmt", numOfStmt);
						}
					|	stmt 
						{
							$$ = new ASTnode("type", "program");
							$$->Set(0.0, $1);
							$$->Set("numOfStmt", 1.0);
							$$->SetLine(yyget_lineno(scanner));
						}
						;


program : program stmt
				{ 
					$$ = $1;
					double numOfStmt = $$->GetValue("numOfStmt")->GetNumberValue();
					$$->Set(numOfStmt, $2);
					numOfStmt++;
					$$->Set("numOfStmt", numOfStmt);
				}
		|	{
				$$ = new ASTnode("type", "program");
				$$->Set("numOfStmt", 0.0);
				$$->SetLine(yyget_lineno(scanner));
				//ast->SetRoot($$);
			}
		;

stmt : expr SEMICOLON	{
							$$ = new ASTnode();
							$$->Set("type", "exprSemicolon");
							$$->Set("expr", $1);
						}		
	| ifstmt			{
							$$ = $1;
						}
	| whilestmt			{
							$$ = $1;
						}
	| forstmt			{
							$$ = $1;
						}
	| returnstmt		{	
							$$ = $1;
						}
	| BREAK SEMICOLON	{
							$$ = new ASTnode();
							$$->Set("type", "break");
							$$->SetLine(yyget_lineno(scanner));
						}
	| CONTINUE SEMICOLON	{
								$$ = new ASTnode();
								$$->Set("type", "continue");
								$$->SetLine(yyget_lineno(scanner));
							}
	| block 			{
								$$ = $1;
							}
	| funcdef				{
								$$ = $1;
							}
	| SEMICOLON				{	
								$$ = new ASTnode();
								$$->Set("type", "semicolon");
								$$->SetLine(yyget_lineno(scanner));
							}
	;

expr :	 assignexpr	{ $$ = $1;}
	| expr PLUS expr	{
							$$ = new ASTnode("type", "add");
							$$->Set("left", $1);
							$$->Set("right", $3);
							$$->SetLine(yyget_lineno(scanner));
						}
	| expr MINUS expr	{
							$$ = new ASTnode("type", "sub");
							$$->Set("left", $1);
							$$->Set("right", $3);
							$$->SetLine(yyget_lineno(scanner));
						}
	| expr MULTI expr	{
							$$ = new ASTnode("type", "mul");
							$$->Set("left", $1);
							$$->Set("right", $3);
							$$->SetLine(yyget_lineno(scanner));
						}
	| expr DIV expr		{
							$$ = new ASTnode("type", "div");
							$$->Set("left", $1);
							$$->Set("right", $3);
							$$->SetLine(yyget_lineno(scanner));
						}
	| expr MOD expr		{
							$$ = new ASTnode("type", "mod");
							$$->Set("left", $1);
							$$->Set("right", $3);
							$$->SetLine(yyget_lineno(scanner));
						}
	| expr GREATER expr	{
							$$ = new ASTnode("type", "greater");
							$$->Set("left", $1);
							$$->Set("right", $3);
							$$->SetLine(yyget_lineno(scanner));
						}
	| expr GREATER_OR_EQUAL expr	{
										$$ = new ASTnode("type", "greater_or_equal");
										$$->Set("left", $1);
										$$->Set("right", $3);
										$$->SetLine(yyget_lineno(scanner));
									}
	| expr LESS expr	{
							$$ = new ASTnode("type", "less");
							$$->Set("left", $1);
							$$->Set("right", $3);
							$$->SetLine(yyget_lineno(scanner));
						}
	| expr LESS_OR_EQUAL expr		{
										$$ = new ASTnode("type", "less_or_equal");
										$$->Set("left", $1);
										$$->Set("right", $3);
										$$->SetLine(yyget_lineno(scanner));
									}
	| expr EQUAL expr	{
							$$ = new ASTnode("type", "equal");
							$$->Set("left", $1);
							$$->Set("right", $3);
							$$->SetLine(yyget_lineno(scanner));
						}
	| expr NOT_EQUAL expr	{
								$$ = new ASTnode("type", "not_equal");
								$$->Set("left", $1);
								$$->Set("right", $3);
								$$->SetLine(yyget_lineno(scanner));
							}
	| expr AND expr		{
							$$ = new ASTnode("type", "and");
							$$->Set("left", $1);
							$$->Set("right", $3);
							$$->SetLine(yyget_lineno(scanner));
						}
	| expr OR expr		{
							$$ = new ASTnode("type", "or");
							$$->Set("left", $1);
							$$->Set("right", $3);
							$$->SetLine(yyget_lineno(scanner));
						}
	| term			{	
							$$ = $1;
						}
	| meta			{
							$$ = $1;
						}
	| error 	{}
	;

term :	 LEFT_PARENTHESIS expr RIGHT_PARENTHESIS	
			{
				$$ = new ASTnode("type", "parentheses");
				$$->Set("expr", $2);
				$$->SetLine(yyget_lineno(scanner));
			}
	| MINUS expr %prec UMINUS	
			{
				$$ = new ASTnode("type", "uminus");
				$$->Set("expr", $2);
				$$->SetLine(yyget_lineno(scanner));
			}
	| NOT expr
			{
				$$ = new ASTnode("type", "not");
				$$->Set("expr", $2);
				$$->SetLine(yyget_lineno(scanner));
			}
	| INCREMENT lvalue
			{
				$$ = new ASTnode("type", "pre_increment");
				$$->Set("lvalue", $2);
				$$->SetLine(yyget_lineno(scanner));
			}
	| lvalue INCREMENT 
			{
				$$ = new ASTnode("type", "post_increment");
				$$->Set("lvalue", $1);
				$$->SetLine(yyget_lineno(scanner));
			}
	| DECREMENT lvalue
			{
				$$ = new ASTnode("type", "pre_decrement");
				$$->Set("lvalue", $2);
				$$->SetLine(yyget_lineno(scanner));
			}
	| lvalue DECREMENT
			{
				$$ = new ASTnode("type", "post_decrement");
				$$->Set("lvalue", $1);
				$$->SetLine(yyget_lineno(scanner));
			}
	| primary	{	
					$$ = $1;
				}
	;

assignexpr : lvalue ASSIGN expr 
				{
					$$ = new ASTnode("type", "assignexpr");
					$$->Set("lvalue", $1);
					$$->Set("expr", $3);
					$$->SetLine(yyget_lineno(scanner));
				}
				;

primary : lvalue	
				{
					$$ = $1;
				}
	| call		{
					$$ = $1;
				}
	| objectdef	{	
					$$ = $1;
				}
	| LEFT_PARENTHESIS funcdef RIGHT_PARENTHESIS	
				{ 
					$$ = $2;
					$$->Set("type", "parentheses_funcdef");
				}
	| const	{
				$$ = $1;
			}
	;

lvalue : IDENT	
			{
				$$ = new ASTnode("type", "var");
				$$->Set("ID", *$1);
				$$->SetLine(yyget_lineno(scanner));
				delete $1;
			}
	| LOCAL IDENT	
			{
				$$ = new ASTnode("type", "localVar");					
				$$->Set("ID", *$2);
				$$->SetLine(yyget_lineno(scanner));
				delete $2; 
			}
	| SCOPE IDENT	
			{
				$$ = new ASTnode("type", "globalVar");		
				$$->Set("ID", *$2);
				$$->SetLine(yyget_lineno(scanner));
				delete $2; 
			}
	| member	
			{ 
				$$ = $1; 
			}
			;

member : lvalue DOT IDENT
				{
					$$ = new ASTnode("type", "member_lvalueVar");
					$$->Set("lvalue", $1);
					$$->Set("ID", *$3);
					$$->SetLine(yyget_lineno(scanner));
					delete $3; 
				
				}
	| lvalue LEFT_SQUARE_BRACKET expr RIGHT_SQUARE_BRACKET 
				{
					$$ = new ASTnode("type", "member_lvalueBrackets");
					$$->Set("lvalue", $1);
					$$->Set("expr", $3);
					$$->SetLine(yyget_lineno(scanner));
				}
	| call DOT IDENT 
				{
					$$ = new ASTnode("type", "member_callVar");
					$$->Set("call", $1);
					$$->Set("ID", *$3);
					$$->SetLine(yyget_lineno(scanner));
					delete $3; 
				}
	| call LEFT_SQUARE_BRACKET expr RIGHT_SQUARE_BRACKET
				{
					$$ = new ASTnode("type", "member_callBrackets");
					$$->Set("call", $1);
					$$->Set("expr", $3);
					$$->SetLine(yyget_lineno(scanner));
				}
	;

call : call LEFT_PARENTHESIS argList RIGHT_PARENTHESIS 
				{
					$$ = new ASTnode("type", "multiCall");
					$$->Set("call", $1);
					$$->Set("argList", $3);
					$$->SetLine(yyget_lineno(scanner));
				}
	| lvalue normcall 
				{
					$$ = new ASTnode("type", "lvalueNormCall");
					$$->Set("lvalue", $1);
					$$->Set("argList", $2);
					$$->SetLine(yyget_lineno(scanner));
				}
	| lvalue methodcall 
				{
					$$ = $2; 
					$$->Set("type", "lvalueMethodCall");
					$$->Set("lvalue", $1);
				}
	| LEFT_PARENTHESIS funcdef RIGHT_PARENTHESIS LEFT_PARENTHESIS argList RIGHT_PARENTHESIS
				{
					$$ = new ASTnode("type", "funcdefCall");
					$$->Set("funcdef", $2);
					$$->Set("argList", $5);
					$$->SetLine(yyget_lineno(scanner));
				}
	;


normcall: LEFT_PARENTHESIS argList RIGHT_PARENTHESIS 
			{
				$$ = $2; 
			}
			;

methodcall : DOUBLE_DOT IDENT LEFT_PARENTHESIS argList RIGHT_PARENTHESIS 
			{
				$$ = new ASTnode("type", "methodcall");
				$$->Set("ID", *$2); 
				$$->Set("argList", $4); 
				$$->SetLine(yyget_lineno(scanner));
				delete $2;
			}
			;

arg : expr	{ 
				$$ = $1;
			} 
	| IDENT COLON expr
			{ 
				$$ = new ASTnode("type", "namedParam");
				$$->Set("expr", $3);
				$$->Set("ID", *$1);
				$$->SetLine(yyget_lineno(scanner));
				delete $1;
			} 
			;

argList : arg 
			{ 
				$$ = new ASTnode("type", "argList");
				$$->Set("numOfTotalArgs", 1.0);
				$$->Set("numOfPositionalArgs", 0.0);
				$$->Set("PositionalArgs", new Object());
				$$->Set("NamedArgs", new Object());
				$$->SetLine(yyget_lineno(scanner));
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
						 interpreter::lineNumber = yyget_lineno(scanner);
						throw RuntimeErrorException("Positional parameter after named parameter"); 
					}
					$$->GetValue("PositionalArgs")->GetObjectValue()->Set(numOfPositionalArgs, $3);
					numOfPositionalArgs++;
					$$->Set("numOfPositionalArgs", numOfPositionalArgs);
				}
				numOfTotalArgs++;
				$$->Set("numOfTotalArgs", numOfTotalArgs);
			}
	|		{
				$$ = new ASTnode("type", "emptyArgList");
				$$->SetLine(yyget_lineno(scanner));
			}
	;

elist :  expr 
			{
				$$ = new ASTnode("type", "elist");
				$$->Set("numOfExprs", 1.0);
				$$->Set(0.0, $1);
				$$->SetLine(yyget_lineno(scanner));
			}
	| elist COMMA expr  
			{
				$$ = $1;
				double numOfExprs = $$->GetValue("numOfExprs")->GetNumberValue();
				$$->Set(numOfExprs, $3);
				numOfExprs++;
				$$->Set("numOfExprs", numOfExprs);
			}
	|		{
				$$ = new ASTnode("type", "emptyElist");
				$$->SetLine(yyget_lineno(scanner));
			}
	;

objectdef : LEFT_SQUARE_BRACKET elist RIGHT_SQUARE_BRACKET 
			{
				$$ = new ASTnode("type", "elistObjectdef");
				$$->Set("elist", $2);
				$$->SetLine(yyget_lineno(scanner));
			}
		| LEFT_SQUARE_BRACKET indexed RIGHT_SQUARE_BRACKET	
			{
				$$ = new ASTnode("type", "indexedObjectdef");
				$$->Set("indexed", $2);
				$$->SetLine(yyget_lineno(scanner));
			}
			;

indexed : indexedelem
			{
				$$ = new ASTnode("type", "indexed");
				$$->Set("numOfElems", 1.0);
				$$->Set(0.0, $1); 
				$$->SetLine(yyget_lineno(scanner));
			}
	| indexed COMMA indexedelem	
			{
				$$ = $1;
				double numOfElems = $$->GetValue("numOfElems")->GetNumberValue();
				$$->Set(numOfElems, $3);
				numOfElems++;
				$$->Set("numOfElems", numOfElems);
			}
			;

indexedelem : LEFT_BRACKET expr COLON expr RIGHT_BRACKET  
				{
					$$ = new ASTnode("type", "indexedElem");
					$$->Set("keyExpr", $2);
					$$->Set("valueExpr", $4);
					$$->SetLine(yyget_lineno(scanner));
				}
				;

tmp_block: tmp_block stmt
				{
					$$ = $1;
					double numOfStmt = $$->GetValue("numOfStmt")->GetNumberValue();
					$$->Set(numOfStmt, $2);
					numOfStmt++;
					$$->Set("numOfStmt", numOfStmt);
				}
		|		{ 
					$$ = new ASTnode("type", "block");
					$$->Set("numOfStmt", 0.0);
					$$->SetLine(yyget_lineno(scanner));
				}
		;

block : LEFT_BRACKET tmp_block RIGHT_BRACKET
			{
				$$ = $2; 
			}
		;


funcbody: LEFT_BRACKET tmp_block RIGHT_BRACKET
				{
					$$ = $2; 
					$2->Set("type", "funcbody");
				}
		;

funcdef : FUNCTION IDENT LEFT_PARENTHESIS idlist RIGHT_PARENTHESIS funcbody
				{
					$$ = new ASTnode("type", "funcdef");
					$$->Set("ID", *$2);
					$$->SetLine(yyget_lineno(scanner));
					ASTnode* funcEnter = new ASTnode("type", "funcEnter");
					funcEnter->Set("idlist", $4);
					funcEnter->Set("funcbody", $6);
					//funcEnter->Set("AddFirst);
					//funcEnter->Set("AddAtAllExitPoints",);
					funcEnter->SetLine(yyget_lineno(scanner));
					$$->Set("funcEnter", funcEnter);
					delete $2;
				}
		| FUNCTION LEFT_PARENTHESIS idlist RIGHT_PARENTHESIS funcbody
				{
					$$ = new ASTnode("type", "anonymousFuncdef");
					ASTnode* funcEnter = new ASTnode("type", "funcEnter");
					funcEnter->Set("idlist", $3);	
					funcEnter->Set("funcbody", $5);
					funcEnter->SetLine(yyget_lineno(scanner));
					$$->Set("funcEnter", funcEnter);
					$$->SetLine(yyget_lineno(scanner));
				}
		; 

const : INTEGER {
					$$ = new ASTnode("type", "numConst");
					$$->Set("value", $1);
					$$->SetLine(yyget_lineno(scanner));
				}
		| REALNUMBER {
						$$ = new ASTnode("type", "numConst");
						$$->Set("value", $1);
						$$->SetLine(yyget_lineno(scanner));
					}
		| STRING {
					$$ = new ASTnode("type", "stringConst");
					$$->Set("value", *$1);
					$$->SetLine(yyget_lineno(scanner));
					delete $1;
				}
		| TRUE 	{
					$$ = new ASTnode("type", "boolConst");
					$$->Set("value", true);
					$$->SetLine(yyget_lineno(scanner));
				}
		| FALSE {
					$$ = new ASTnode("type", "boolConst");
					$$->Set("value", false);
					$$->SetLine(yyget_lineno(scanner));
				}
		| NIL 	{
					$$ = new ASTnode("type", "nil");
					$$->SetLine(yyget_lineno(scanner));
				}
		;

formal: IDENT 
		{
			$$ = new ASTnode("type", "param");
			$$->Set("ID", *$1);
			$$->SetLine(yyget_lineno(scanner));
			delete $1;
		}
	
	| IDENT ASSIGN expr
		{
			$$ = new ASTnode("type", "optionalParam");
			$$->Set("ID", *$1);
			$$->Set("expr", $3);
			$$->SetLine(yyget_lineno(scanner));
			delete $1;
		}
	;


idlist : formal 
		{
			$$ = new ASTnode("type", "idlist");
			$$->Set("numOfParams", 1.0);
			$$->Set(0.0, $1); 
			$$->SetLine(yyget_lineno(scanner));
			}
	| idlist COMMA formal	
			{
				$$ = $1;
				double numOfParams = $$->GetValue("numOfParams")->GetNumberValue();
				$$->Set(numOfParams, $3);
				numOfParams++;
				$$->Set("numOfParams", numOfParams);
			}
	|		{
				$$ = new ASTnode("type", "emptyIdlist");
				$$->Set("numOfParams", 0.0);
				$$->SetLine(yyget_lineno(scanner));
			}
			;

ifstmt : IF LEFT_PARENTHESIS expr RIGHT_PARENTHESIS stmt 
			{
				$$ = new ASTnode("type", "ifstmt");
				$$->Set("condition", $3);					
				$$->Set("stmt", $5);
				$$->SetLine(yyget_lineno(scanner));

			}
		| ifstmt ELSE stmt 
			{
				$$ = new ASTnode("type", "if_elsestmt");
				$$->Set("ifstmt", $1);					
				$$->Set("elsestmt", $3);
				$$->SetLine(yyget_lineno(scanner));
			}
			;

whilestmt : WHILE LEFT_PARENTHESIS expr RIGHT_PARENTHESIS stmt 
			{
				$$ = new ASTnode("type", "whilestmt");
				$$->Set("condition", $3);
				$$->Set("stmt", $5);	
				$$->SetLine(yyget_lineno(scanner));
			}
			;

forstmt : FOR LEFT_PARENTHESIS elist SEMICOLON expr SEMICOLON elist RIGHT_PARENTHESIS stmt
			{
				$$ = new ASTnode("type", "forstmt");
				$$->Set("init_elist", $3);	
				$$->Set("condition", $5);
				$$->Set("elist", $7);
				$$->Set("stmt", $9);	
				$$->SetLine(yyget_lineno(scanner));
			}
			;

returnstmt : RETURN SEMICOLON 
				{
					$$ = new ASTnode("type", "return");
					$$->SetLine(yyget_lineno(scanner));
				}
		| RETURN expr SEMICOLON 
				{
					$$ = new ASTnode("type", "return_value");
					$$->Set("expr", $2);
					$$->SetLine(yyget_lineno(scanner));
				}
				;


meta:  meta_syntax 
						{
							$$ = $1;
						}
	| meta_escape 
						{
							$$ = $1;
						}
	| meta_execute
						{
							$$ = $1;
						}
	| meta_parse 
						{
							$$ = $1;
						}
	| meta_unparse 
						{
							$$ = $1;
						}


meta_syntax : META_SYNTAX_OPEN expr META_SYNTAX_CLOSE 
						{
							$$ = new ASTnode("type", "meta_syntax");
							$$->SetLine(yyget_lineno(scanner));
							ASTnode* root = new ASTnode("type", "metaAST");
							root->SetLine(yyget_lineno(scanner));
							root->Set("root", $2);
							$$->Set("expr", root);
						}
						;

meta_escape: META_ESCAPE expr
						{
							$$ = new ASTnode("type", "meta_escape");
							$$->SetLine(yyget_lineno(scanner));
							$$->Set("meta_var", $2);
							//$$->Set("meta_var", *$2);
							//delete $2;
						}
						;

meta_execute: META_EXECUTE expr
						{
							$$ = new ASTnode("type", "meta_execute");
							$$->SetLine(yyget_lineno(scanner));
							$$->Set("expr", $2);
						}
						;

meta_parse: META_PARSE expr
						{
							$$ = new ASTnode("type", "meta_parse");
							$$->SetLine(yyget_lineno(scanner));
							$$->Set("expr", $2);
						}
						;

meta_unparse: META_UNPARSE expr
						{
							$$ = new ASTnode("type", "meta_unparse");
							$$->SetLine(yyget_lineno(scanner));
							$$->Set("expr", $2);
						}
						;
%%

int yyerror(AST* ast, yyscan_t scanner, int isEval, const char *yaccProvidedMessage){
	std::string yaccProvidedMessage_string = yaccProvidedMessage;
	 interpreter::lineNumber =  yyget_lineno(scanner);
	throw SyntaxErrorException(yaccProvidedMessage_string + " undefined token  \" "  + yyget_text(scanner) + "\"");
	return 0;
}