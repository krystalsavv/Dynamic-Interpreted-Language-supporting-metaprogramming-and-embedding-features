%{
	#include <stdio.h>
	#include <stdlib.h>
	#include <string.h>
	#include "header.h"
	#include "parser.h"
%}

%option noyywrap 
%option yylineno 
%option case-insensitive

keyword 	(function)|(while)|(for)|(if)|(else)|(continue)|(break)|(return)|(and)|(or)|(not)|(local)|(true)|(false)|(nil)
operator 	[\*\/\%]|(\={1,2})|(\>\=?)|(\<\=?)|(\!\=)|(\+{1,2})|(\-{1,2})
integer 	(0[xX][0-9A-Fa-f]+)|([0-9]+)
real 		[0-9]+\.[0-9](e|E)?[0-9]*	
space 		[\r\n \t\v]
string  	\"[^\n"]*\"
id 			[a-zA-Z][a-zA-Z_0-9]* 
comment1 	"//".*
comment2 	"#".*
punc 		[\[\]\{\}\(\)\;\,]|(\:{1,2})|(\.{1,2})


%%

"\"" {
	char c;
	int count = 0, startline = yylineno;
	char *str = malloc(sizeof(char));
	while((c=input())!=EOF){
		if(c == '\\'){
			if((c=input())!=EOF){
				if(c == 'n'){
					str[count] = '\n';
					count++;
					str = realloc(str,count*sizeof(char));
				}
				else if(c ==  't'){
					str[count] = '\t';
					count++;
				str = realloc(str,count*sizeof(char));
				}
				else if(c ==  'r'){
					str[count] = '\r';
					count++;
				str = realloc(str,count*sizeof(char));
				}
				else if(c ==  '\\'){
					str[count] = '\\';
					count++;
				str = realloc(str,count*sizeof(char));
				}
				else if(c == '"'){
					str[count] = '\"';
					count++;
				str = realloc(str,count*sizeof(char));
				}
				else {
					str[count] = c;
					count++;
				str = realloc(str,count*sizeof(char));
					fprintf(yyerror,"WARNING: invalid escape character %c in line %d\n",c,yylineno);
				}
			}
			else{
				fprintf(yyerror,"EOF\n");
				unput(c);
			}
		}
		else if(c == '"'){
			str[count] = '\0';
			count++;
			str = realloc(str,count*sizeof(char));
			break;
		}
		else {
			str[count] = c;
			count++;
			str = realloc(str,count*sizeof(char));
		}
	}
	if(c == EOF){
		//token_counter++;
		//yval = list_insert(yval,startline,token_counter,ERROR,"not closed string");
		fprintf(yyerror, "ERROR: not closed string")				// krystalsav 
	}
	else{
		yylval.stringVal=yytext;
		return STRING;
		//token_counter++;
		//yval = list_insert(yval,startline,token_counter,STRING,str);
	}

}

"/*" {
	void comm_func(int number){
		int c;
		
		while((c=input()) != EOF){
			if(c=='/'){
				if((c=input()) =='*') comm_func(number+1);
				else unput(c);
			}			
			else if(c=='*'){
				if((c=input()) =='/') break;
				else unput(c);
			}
		}
		//token_counter++; 
		if(number==0){
			if(c != EOF) {//yval=list_insert(yval,yylineno,token_counter,MULTI_LINE_COMMENT,yytext);
			}
			else{ //yval=list_insert(yval,yylineno,token_counter,ERROR,"not closed0 comments");
				fprintf(yyerror, "ERROR: not closed comments");			// krystalsav
			}
		}else{
			if(c != EOF) {
				//yval=list_insert(yval,yylineno,token_counter,NESTED_COMMENT,yytext);
			}
			else { //yval=list_insert(yval,yylineno,token_counter,ERROR,"not closed nested comments");
			fprintf(yyerror, "ERROR: not closed nested comments");	// krystalsav	
			}
		}
	}
	comm_func(0);
} 

{keyword}	{return keywords_to_enum(yytext);}
{operator}	{return keywords_to_enum(yytext);}
{integer} 	{yylval.intVal = atoi(yytext);return INTEGER;}
{real} 		{yylval.doubleVal = atof(yytext);return REALNUMBER;}
{space}		{}
{comment1}	{}
{comment2}	{}
{punc}		{return keywords_to_enum(yytext);}
{id}		{yylval.stringVal=strdup(yytext);return IDENT;}
. 			{fprintf(yyerror,"Line %d: Lexical analysis error:You can not have an identifier starting with number or symbol $ (WRONG DEFINITION). \n",yylineno);return WRONG_DEFINITION;}

%%

int keywords_to_enum(char* text){

	if(!strcmp(text,"if")) 				return IF;
	else if(!strcmp(text,"else")) 		return ELSE;
	else if(!strcmp(text,"while")) 		return WHILE;
	else if(!strcmp(text,"for")) 		return FOR;
	else if(!strcmp(text,"function")) 	return FUNCTION;
	else if(!strcmp(text,"return"))	 	return RETURN;
	else if(!strcmp(text,"break")) 		return BREAK;
	else if(!strcmp(text,"continue")) 	return CONTINUE;
	else if(!strcmp(text,"and"))		return AND;
	else if(!strcmp(text,"not"))		return NOT;
	else if(!strcmp(text,"or")) 		return OR;	
	else if(!strcmp(text,"local")) 		return LOCAL;
	else if(!strcmp(text,"true")) 		return TRUE;
	else if(!strcmp(text,"false")) 		return FALSE;
	else if(!strcmp(text,"nil")) 		return NIL;
	else if(strcmp(text,"+")==0)		return  PLUS;
	else if (strcmp(text,"=")==0)		return ASSIGN;
	else if (strcmp(text,"-")==0)		return MINUS;
	else if (strcmp(text,"*")==0)		return MULTI;
	else if (strcmp(text,">=")==0)		return GREATER_OR_EQUAL;
	else if (strcmp(text,"<=")==0)		return  LESS_OR_EQUAL;
	else if (strcmp(text,">")==0)		return GREATER;
	else if (strcmp(text,"<")==0)		return LESS;
	else if (strcmp(text,"/")==0)		return  DIV;
	else if (strcmp(text,"%")==0)		return MOD;
	else if (strcmp(text,"==")==0)		return EQUAL;
	else if (strcmp(text,"!=")==0)		return NOT_EQUAL;
	else if (strcmp(text,"++")==0)		return INCREMENT;
	else if (strcmp(text,"--")==0)		return DECREMENT;
	else if(strcmp(text,"{")==0)		return LEFT_BRACKET;
	else if(strcmp(text,"}")==0)		return RIGHT_BRACKET;
	else if(strcmp(text,"[")==0)		return LEFT_SQUARE_BRACKET;
	else if(strcmp(text,"]")==0)		return RIGHT_SQUARE_BRACKET;	
	else if(strcmp(text,"(")==0)		return LEFT_PARENTHESIS;
	else if(strcmp(text,")")==0)		return RIGHT_PARENTHESIS;
	else if(strcmp(text,";")==0)		return SEMICOLON;	
	else if(strcmp(text,",")==0)		return COMMA;
	else if(strcmp(text,":")==0)		return COLON;
	else if(strcmp(text,"::")==0)		return SCOPE;
	else if(strcmp(text,".")==0)		return DOT;
	else if(strcmp(text,"..")==0)		return DOUBLE_DOT;	
}

	
