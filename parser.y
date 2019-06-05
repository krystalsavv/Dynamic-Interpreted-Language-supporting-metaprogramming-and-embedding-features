%{
	#include <stdio.h>
	#include <string.h>
	#include <stdlib.h>
	#include "hashTable.c"
	int yyerror (const char* yaccProvidedMessage);
	int yylex (void);
	extern int yylineno;
	extern char* yytext;
	extern FILE* yyin;
	extern FILE* yyout;
	extern FILE* yyerror;	

	int scope =  0;	
	int func_scope = 0;	// kataei to bathos thn pio "bathias" function
	int loop_scope = 0; 	// elegxei an yparxei break/continue ektws loop
	int func_flag = 0;
	int call_flag = 0;
	int lib_flag = 0;
	int fcount = 0;
	SymbolTableEntry *func_tmp;
	
%}


%union{
	char *stringVal;
	int intVal;
	double doubleVal;
}
%error-verbose
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
				if(func_scope == 0)
					fprintf(yyout,"Syntax Error in line %d :RETURN outside of any function.\n",yylineno);
			}
	| BREAK SEMICOLON	
			{
				printf("BREAK\n");
				if(loop_scope == 0)
					fprintf(yyout,"Syntax Error in line %d :BREAK outside of any loop.\n",yylineno);
			}
	| CONTINUE SEMICOLON	
			{
				printf("CONTINUE\n");
				if(loop_scope == 0)
					fprintf(yyout,"Syntax Error in line %d :CONTINUE outside of any loop.\n",yylineno);
			}
	| block 	{printf("Block\n");}
	| funcdef	{printf("Function definition\n");}
	| SEMICOLON	{printf("SEMICOLON ;\n");}
	;

expr :	 assignexpr		{}
	| expr PLUS expr	{printf("expression  + expression -> %d+%d\n",$1,$3); 	$$ = $1 + $3;}
	| expr MINUS expr	{printf("expression  - expression -> %d-%d\n",$1,$3); 	$$ = $1 - $3;}
	| expr MULTI expr		{printf("expression  * expression -> %d*%d\n",$1,$3); 	$$ = $1 * $3;}
	| expr DIV expr		{printf("expression  / expression -> %d/%d\n",$1,$3); 	$$ = $1 / $3;}
	| expr MOD expr		{printf("expression %% expression -> %d %% %d\n",$1,$3); 	$$ = $1 % $3;}
	| expr GREATER expr	{printf("expression  > expression -> %d>%d\n",$1,$3); 	$$ = ($1 > $3)?1:0;}
	| expr GREATER_OR_EQUAL expr	{printf("expression >= expression -> %d>=%d\n",$1,$3); 	$$ = ($1>=$3)?1:0;}
	| expr LESS expr	{printf("expression  < expression -> %d<%d\n",$1,$3); 	$$ = ($1<$3)?1:0;}
	| expr LESS_OR_EQUAL expr	{printf("expression <= expression -> %d<=%d\n",$1,$3); 	$$ = ($1<=$3)?1:0;}
	| expr EQUAL expr		{printf("expression == expression -> %d==%d\n",$1,$3); 	$$ = ($1==$3)?1:0;}
	| expr NOT_EQUAL expr	{printf("expression != expression -> %d!=%d\n",$1,$3); 	$$ = ($1!=$3)?1:0;}
	| expr AND expr		{printf("expression && expression -> %d&&%d\n",$1,$3); 	$$ = ($1&&$3)?1:0;}
	| expr OR expr		{printf("expression || expression -> %d/%d\n",$1,$3); 	$$ = ($1||$3)?1:0;}
	| term			{printf("Terminal\n");}
	| error 	{}
	;

term :	 LEFT_PARENTHESIS expr RIGHT_PARENTHESIS	{printf("( expression )\n");}
	| MINUS expr %prec UMINUS	
			{
				printf("- expression\n");
				if(func_flag == 1)
				{
					func_flag = 0;
					fprintf(yyout,"Syntax Error in line %d :Cannot do operetions to functions.\n",yylineno);  
				}
			}
	| NOT expr
			{
				printf("!expression\n");
				if(func_flag == 1)
				{
					func_flag = 0;
					fprintf(yyout,"Syntax Error in line %d :Cannot do operetions to functions.\n",yylineno);  
				}
			}
	| INCREMENT lvalue
			{
				printf("++ lvalue\n");
				if(func_flag == 1)
				{
					func_flag = 0;
					fprintf(yyout,"Syntax Error in line %d :Cannot do operetions to functions.\n",yylineno);  
				}
			}
	| lvalue INCREMENT 
			{
				printf("lvalue ++\n");
				if(func_flag == 1)
				{
					func_flag = 0;
					fprintf(yyout,"Syntax Error in line %d :Cannot do operetions to functions.\n",yylineno);  
				}
			}
	| DECREMENT lvalue
			{
				printf("-- lvalue\n");
				if(func_flag == 1)
				{
					func_flag = 0;
					fprintf(yyout,"Syntax Error in line %d :Cannot do operetions to functions.\n",yylineno);  
				}
			}
	| lvalue DECREMENT
			{
				printf("lvalue --\n");
				if(func_flag == 1)
				{
					func_flag = 0;
					fprintf(yyout,"Syntax Error in line %d :Cannot do operetions to functions.\n",yylineno);  
				}
			}
	| primary	{printf("Primary\n");}
	;

assignexpr : lvalue ASSIGN expr {printf("lvalue = expression \n");
					if(func_flag == 1)
						fprintf(yyout,"Syntax Error in line %d :Cannot do assignment to functions.\n",yylineno);
					func_flag = 0;
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

				SymbolTableEntry *sym_ret = lookupHT($1, func_scope);
				if(sym_ret == NULL){
					if(scope!=0)
						insertHT($1, scope, yylineno, func_scope, LOCAL_V);
					else
						insertHT($1, scope, yylineno, func_scope, GLOBAL);
				}
				else{
					if(sym_ret->func_scope != func_scope && scopeST(*sym_ret)!=0)	{
						if(sym_ret->type!=USERFUNC && sym_ret->type!=LIBFUNC)
							fprintf(yyout,"Syntax Error in line %d :%s no access to this variable.\n",yylineno, $1);					
					}
					if(sym_ret->type==USERFUNC) { 
						func_flag = 1;	
					}	
					else if(sym_ret->type==LIBFUNC){ 
						func_flag = 1;
						lib_flag = 1;
					}
				}
			}
	| LOCAL IDENT	
			{
				printf("Local ID\n");
				
				SymbolTableEntry *sym_ret = lookupHT($2, func_scope);
				if(sym_ret == NULL){
					if(scope==0)
						insertHT($2, scope, yylineno, func_scope, GLOBAL);
					else
						insertHT($2, scope, yylineno, func_scope, LOCAL_V);
				}
				else if(scope!=scopeST(*sym_ret)){
					if(sym_ret->type == LIBFUNC) fprintf(yyout,"Syntax Error in line %d :%s is lib function.\n",yylineno, $2);
					else insertHT($2, scope, yylineno, func_scope, LOCAL_V);
				}

			}
	| SCOPE IDENT	
			{
				printf("::ID\n");
				SymbolTableEntry *sym_ret=lookup_scopelist($2,0);//psaxnei ta global
				if(sym_ret == NULL){
					fprintf(yyout,"Syntax Error in line %d :%s is not a global.\n",yylineno, $2);
				}						
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

callsuffix : normcall {printf("normcall\n");func_flag=0;lib_flag=0;}
	| methodcall {printf("methodcall\n");func_flag=0;lib_flag=0;}
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

indexedelem : LEFT_BRACKET{scope++;} expr COLON expr RIGHT_BRACKET  {printf("{ expresion : expresion }\n");
							hideHT(scope--);
				}
			;

tmp_block: tmp_block stmt 
		| {}
		;

block : LEFT_BRACKET{scope++;} tmp_block RIGHT_BRACKET{ printf("{ stmt }\n");
														hideHT(scope--);
													  }
		;

funcdef : FUNCTION IDENT 
				{
					SymbolTableEntry *sym_ret = lookupHT($2, func_scope);
					if(sym_ret == NULL){
						insertHT($2, scope, yylineno, func_scope, USERFUNC);
						func_tmp=lookupHT($2, func_scope);
					}
					else if(scopeST(*sym_ret)==scope)
						if(sym_ret->type==LIBFUNC)
							fprintf(yyout,"Syntax Error in line %d :Cannot redeclear a Lib Function.\n",yylineno);
						else							
							fprintf(yyout,"Syntax Error in line %d :Cannot declear this Function,already in this scope.\n",yylineno);
					else {
						if(sym_ret->type==LIBFUNC)
							fprintf(yyout,"Syntax Error in line %d :Cannot redeclear a Lib Function.\n",yylineno);
						else{
							insertHT($2, scope, yylineno, func_scope, USERFUNC);
							func_tmp=lookupHT($2, func_scope);
						}
					}
					func_scope++;				
				}
			
			LEFT_PARENTHESIS idlist RIGHT_PARENTHESIS block
				{
					func_scope--;
					printf("function id(idlist) block\n");
				}
		| FUNCTION 
				{
					char *name = Anonymous_func_name(++fcount);
					insertHT(name, scope, yylineno, func_scope, USERFUNC);
					func_tmp=lookupHT(name,func_scope);
					func_scope++;				
				}
			LEFT_PARENTHESIS idlist RIGHT_PARENTHESIS block
				{
					func_scope--;
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
			SymbolTableEntry *sym_ret = lookupHT($1, func_scope);
			if(sym_ret == NULL){
				insertHT($1, scope+1, yylineno, func_scope, FORMAL);
				ArgInsert(func_tmp, $1);
			}
			else if (scope+1==scopeST(*sym_ret)){
				fprintf(yyout,"Syntax Error in line %d :Cannot redeclear a parametre with this name.\n",yylineno);
			}
			else{
				if(sym_ret->type==LIBFUNC)
					fprintf(yyout,"Syntax Error in line %d :Cannot redeclear a Lib Function as a parametre.\n",yylineno);
				else{
					insertHT($1, scope+1, yylineno, func_scope, FORMAL);
					ArgInsert(func_tmp, $1);
				}		
			}								
		}
	| idlist COMMA IDENT  
		{
			printf("idlist, id\n");
			SymbolTableEntry *sym_ret = lookupHT($3, func_scope);
			if(sym_ret == NULL){
				insertHT($3, scope+1, yylineno, func_scope, FORMAL);
				ArgInsert(func_tmp, $3);
			}
			else if (scope+1==scopeST(*sym_ret)){
				fprintf(yyout,"Syntax Error in line %d :Cannot redeclear a parametre with this name.\n",yylineno);
			}
			else{
				if(sym_ret->type==LIBFUNC)
					fprintf(yyout,"Syntax Error in line %d :Cannot redeclear a Lib Function as a parametre.\n",yylineno);
				else{
					insertHT($3, scope+1, yylineno, func_scope, FORMAL);
					ArgInsert(func_tmp, $3);
				}		
			}		
		}
	|  {printf("Empty (idlist)\n");}
	;

ifstmt : IF LEFT_PARENTHESIS expr RIGHT_PARENTHESIS stmt {printf("IF (expresion) stmt\n");}
		| ifstmt ELSE stmt {printf("ifstmt ELSE stmt\n");}
		;

whilestmt : WHILE
			{
				printf("While (expresion) stmt\n");
				loop_scope++;
			}
			 LEFT_PARENTHESIS expr RIGHT_PARENTHESIS stmt 
			{
				loop_scope--;
			}
			
		;

forstmt : FOR 
			{
				printf("for (elist;elist;elist) stmt\n");
				loop_scope++;
			}
			LEFT_PARENTHESIS elist SEMICOLON expr SEMICOLON elist RIGHT_PARENTHESIS stmt
			{
				loop_scope--;
			}
		;

returnstmt : RETURN SEMICOLON {printf("RETURN;\n");}
		| RETURN expr SEMICOLON {printf("RETURN expresion;\n");}
		;


%%

int yyerror(const char *yaccProvidedMessage){

	fprintf(yyout,"%s: at line %d before token :%s \n",yaccProvidedMessage,yylineno,yytext);
	fprintf(yyout,"INPUT NOT VALID \n");
	return 0;
}


int main(int argc,char** argv){
	initHT();
	if(argc>1){
		if(!(yyin=fopen(argv[1],"r"))){
				fprintf(stderr,"Cannot read file %s \n",argv[1]);
				return 1;
		}
		yyout = stdout;
		if(argc==3){
			if(!(yyout=fopen(argv[2],"w"))){
				fprintf(stderr,"Cannot write to file %s \n",argv[2]);
				return 1;
			}
		}
	} 
	else {
		yyin =stdin;
		
	}
	yyparse();
	printHT(yyout);
return 0;
}
