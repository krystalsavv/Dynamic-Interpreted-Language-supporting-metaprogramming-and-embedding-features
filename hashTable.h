#ifndef HASHTABLE_H
#define HASHTABLE_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>


typedef struct ArgList{
    char *name;
    struct ArgList *next;
}ArgList;

typedef struct Variable {
	const char *name;
	unsigned int scope;
	unsigned int line;
} Variable;

typedef struct Function {
	const char *name;
	ArgList *args;
	unsigned int scope;
	unsigned int line;
} Function;


typedef enum SymbolTableType {
	GLOBAL, LOCAL_V, FORMAL,USERFUNC, LIBFUNC
}SymbolType;


typedef struct SymbolTableEntry {
	int isActive;
	union {
	Variable *varVal;
	Function *funcVal;
	} value;
	int func_scope;
	SymbolType type;
	struct SymbolTableEntry *next;		// next sthn hashtable list 
	struct SymbolTableEntry *scope_next;	//next sthn scopelist 
} SymbolTableEntry;

typedef struct scopelist {
	int scope;
	SymbolTableEntry *this_scope;
	struct scopelist *next;
}ScopeList;

SymbolTableEntry *hashTable[1000];

ScopeList *scopelisthead=NULL;

int hashFunction(const char *element);

void initHT();

void insertHT(const char *name, unsigned int scope, unsigned int line, int func_scope, SymbolType type);

//SymbolTableEntry * lookupHT(SymbolTableEntry *tmp);

SymbolTableEntry * lookupHT(const char *name, int fscope);

void hideHT(int currScope);

void printHT();

SymbolTableEntry * lookup_scopelist(const char *name,int scope);

/* Epistrefei to name analoga me to type tou SymbolTableEntry */
char * nameST(SymbolTableEntry arg);

/* Epistrefei to scope analoga me to type tou SymbolTableEntry */
int scopeST(SymbolTableEntry arg);

/* Epistrefei to line analoga me to type tou SymbolTableEntry */
unsigned int lineST(SymbolTableEntry arg);

void ArgInsert(SymbolTableEntry* tmp, char *name);

/* Epistrefei onoma gia tis functions poy den to exoyn dhlwsei */
char* Anonymous_func_name(int fcount);

#endif