#ifndef HASHTABLE_H
#define HASHTABLE_H

#include <stdlib.h>
#include <stdio.h>
#include <string>


// typedef struct ArgList{
//     char *name;
//     struct ArgList *next;
// }ArgList;

// typedef struct Variable {
// 	const char *name;
// 	unsigned int scope;
// 	unsigned int line;
// } Variable;

// typedef struct Function {
// 	const char *name;
// 	ArgList *args;
// 	unsigned int scope;
// 	unsigned int line;
// } Function;


// typedef enum SymbolTableType {
// 	GLOBAL, LO_CALV, FORMAL,USERFUNC, LIBFUNC
// }SymbolType;


// typedef struct SymbolTableEntry {
// 	int isActive;
// 	union {
// 	Variable *varVal;
// 	Function *funcVal;
// 	} value;
// 	int func_scope;
// 	SymbolType type;
// 	struct SymbolTableEntry *next;		// next sthn hashtable list 
// 	struct SymbolTableEntry *scope_next;	//next sthn scopelist 
// } SymbolTableEntry;




/* Epistrefei onoma gia tis functions poy den to exoyn dhlwsei */
std::string Anonymous_func_name(int fcount);

#endif