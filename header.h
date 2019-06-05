#ifndef HEADER_H
#define HEADER_H

/*typedef enum type {            
	IF,
	ELSE,
	WHILE,
	FOR,
	FUNCTION,
	RETURN,
	BREAK,
	CONTINUE,
	AND,
	NOT,
	OR,
	LOCAL,
	TRUE,
	FALSE,
	NIL,
	PLUS,
	ASSIGN,
	MINUS,
	MULTI,
	GREATER_OR_EQUAL,
	LESS_OR_EQUAL,
	GREATER,
	LESS,
	DIV,
	MOD,
	EQUAL,
	NOT_EQUAL,
	INCREMENT,
	DECREMENT,
	LEFT_BRACKET,
	RIGHT_BRACKET,
	LEFT_SQUARE_BRACKET,
	RIGHT_SQUARE_BRACKET,	
	LEFT_PARENTHESIS,
	RIGHT_PARENTHESIS,
	SEMICOLON,	
	COMMA,
	COLON,
	SCOPE,
	DOT,
	DOUBLE_DOT,

	IDENT,
	INTEGER,
	REAL,
	STRING,
	LINE_COMMENT,
	PUNC,
	SPACE,
	OPERATOR,
	MULTI_LINE_COMMENT,
	NESTED_COMMENT,
	ERROR
} types;*/	

typedef struct token a_token_t;

struct token{
	int numline;
	int numToken;
	//types type;
	int int_val;
	float float_val;
	char *content;
	a_token_t *next;
};

/* This counter is used for the token no */
// int token_counter = 0;

int keywords_to_enum(char* text);

//const char* get_type(types key);

//a_token_t * list_insert(a_token_t * head,int numline, int numToken,types type, char *text);

// void list_print(a_token_t *head);

#endif
