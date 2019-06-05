#include "hashTable.h"


int hashFunction(const char *element){
	int i = 0, sum = 0;
	while(i<strlen(element))
	{
		sum += (int)element[i++];
	}
	return sum % 1000;
}


void initHT(){	//arxikopoihsh
	int i;
	for(i=0;i<1000;i++){
		hashTable[i]=NULL;
	}
	insertHT("print",0,0,0,LIBFUNC);
	insertHT("input",0,0,0,LIBFUNC);
	insertHT("objectmemberkeys",0,0,0,LIBFUNC);
	insertHT("objecttotalmembers",0,0,0,LIBFUNC);
	insertHT("objectcopy",0,0,0,LIBFUNC);
	insertHT("totalarguments",0,0,0,LIBFUNC);
	insertHT("argument",0,0,0,LIBFUNC);
	insertHT("typeof",0,0,0,LIBFUNC);
	insertHT("strtonum",0,0,0,LIBFUNC);
	insertHT("sqrt",0,0,0,LIBFUNC);
	insertHT("cos",0,0,0,LIBFUNC);
	insertHT("sin",0,0,0,LIBFUNC);
}


void insertHT(const char *name, unsigned int scope, unsigned int line, int func_scope, SymbolType type){	
	int i = hashFunction(name);
	SymbolTableEntry *node = malloc(sizeof(SymbolTableEntry));
	node->isActive = 1;
	node->type = type;
	node->func_scope = func_scope;
	if(type==LOCAL_V && scope==0) node->type=GLOBAL;
	if(type == USERFUNC || type == LIBFUNC){
		node->value.funcVal = malloc(sizeof(struct Function));
		node->value.funcVal->name = strdup(name);
		node->value.funcVal->scope = scope;
		node->value.funcVal->line = line;
		node->value.funcVal->args = NULL;
	}
	else {
		node->value.varVal = malloc(sizeof(struct Variable));
		node->value.varVal->name = strdup(name);
		node->value.varVal->scope = scope;
		node->value.varVal->line = line;
	}
	node->scope_next = NULL;
	// insert sto hashtable
	node->next = hashTable[i];
	hashTable[i] = node;

	// insert sthn scopelist 
	ScopeList *prev = NULL;
	ScopeList *p = scopelisthead;
	while(p!=NULL && p->scope < scope)
	{	
		prev = p;
		p = p->next;
	}
	if(p == NULL){
		ScopeList *new_scope = malloc(sizeof(ScopeList));
		new_scope->scope = scope;
		new_scope->next = NULL;
		new_scope->this_scope = node;
		if(prev == NULL)
			scopelisthead = new_scope;
		else 
			prev->next = new_scope;
	}
	else{
		if(p->scope!=scope){//prepei na ftia3w allo scope
			p = malloc(sizeof(ScopeList));
			p->scope = scope;
			if(prev==NULL){
				p->next=scopelisthead;
				scopelisthead = p;
			}	
			else{
				p->next=prev->next;
				prev->next=p;
			}
		}
		
		node->scope_next = p->this_scope;
		p->this_scope = node;	
	}
}


SymbolTableEntry * lookupHT(const char *name, int fscope){
	char * head_name;
	int head_scope,head_fscope;
	int i = hashFunction(name);
	SymbolTableEntry * head = hashTable[i];
	for(head;head;head=head->next){//an yparxei kati me to idio name 8a einai se ayth th list tou hashtable
		head_fscope = head->func_scope;
		head_scope = scopeST(*head);
		head_name = nameST(*head);
		//if(!strcmp(head_name,name) && head_scope==0) //global kai libfunc
		//	return head;
		if(head->isActive && !strcmp(head_name,name))//an einai idio scope kai energo kai exoun to idio onoma
			return head; 
	}
	return NULL;
}


void hideHT(int currScope){//kaleitai me scope-- gia na meiw8ei kai to scope otan kalestei
	int i;
	ScopeList * head=scopelisthead;
	while(head!=NULL && head->scope < currScope){
		head=head->next;
	}
	if(head==NULL) return;
	else{
		SymbolTableEntry * tmp;
		for(tmp=head->this_scope;tmp;tmp=tmp->scope_next){
			tmp->isActive=0;
		}
	}
}

static const char * SymbolType_to_string(SymbolType type){
	if(type == GLOBAL) return "GLOBAL";
	else if(type == LOCAL_V) return "LOCAL_V";
	else if (type == FORMAL) return "FORMAL";
	else if (type == USERFUNC) return "USERFUNC";
	else return "LIBFUNC";

}


void printHT(FILE *f){//kaleitai me scope-- gia na meiw8ei kai to scope otan kalestei
	int i,scope;
	char* name;
	ScopeList *head=scopelisthead;
	SymbolTableEntry *tmp;
	fprintf(f,"SymbolTable:\n");
	while(head!=NULL){
		fprintf(f,"SCOPE %d :\n", head->scope);
		for(tmp=head->this_scope;tmp;tmp=tmp->scope_next){
			if(tmp->type == USERFUNC || tmp->type == LIBFUNC){
				name = strdup(tmp->value.funcVal->name);
				scope = tmp->value.funcVal->scope;
			}
			else {
				name = strdup(tmp->value.varVal->name);
				scope = tmp->value.varVal->scope;
			}
			fprintf(f,"name: %s  scope: %d  func_scope: %d  isActive: %d  type: %s\n",name,scope,tmp->func_scope,tmp->isActive,SymbolType_to_string(tmp->type));
		}
		fprintf(f,"------------------------------------------------------------------------\n");
		head=head->next;
	}
}


SymbolTableEntry * lookup_scopelist(const char *name, int scope){
	ScopeList *slist = scopelisthead;
	SymbolTableEntry *p;
	while(slist!=NULL && slist->scope<scope)
	{
		slist = slist->next;
	}
	if(slist==NULL) return NULL;
	p = slist->this_scope;
	while(p!=NULL && strcmp(nameST(*p),name))
	{
		p = p->scope_next;
	}
	return p;
}


char * nameST(SymbolTableEntry arg){
	if(arg.type == USERFUNC || arg.type == LIBFUNC) 
		return strdup(arg.value.funcVal->name);
	else 
		return strdup(arg.value.varVal->name);
}


int scopeST(SymbolTableEntry arg){
	if(arg.type == USERFUNC || arg.type == LIBFUNC) 
		return arg.value.funcVal->scope;
	else 
		return arg.value.varVal->scope;
}


unsigned int lineST(SymbolTableEntry arg){
	if(arg.type == USERFUNC || arg.type == LIBFUNC) 
		return arg.value.funcVal->line;
	else 
		return arg.value.varVal->line;
}

void ArgInsert(SymbolTableEntry* tmp, char *name){
	if(tmp==NULL) return;
	ArgList *new = malloc(sizeof(ArgList));
	new->name = strdup(name);
	new->next = NULL;
	if(tmp->value.funcVal->args == NULL)
		tmp->value.funcVal->args = new;
	else {
		ArgList *p, *prev;
		p = tmp->value.funcVal->args;
		prev = NULL;
		while(p != NULL){
			prev = p;
			p = p->next;
		}
		prev->next = new;
	}
}

char* Anonymous_func_name(int fcount)
{
    int counter=0,tmp=fcount;
	while(fcount>0){
		counter++;
		fcount=fcount-fcount%10;
		fcount=fcount/10;
	}
	char *name=malloc(counter+3);
	strcpy(name,"_f");
	char*c;
	while(counter>0){
		int x=tmp%10;
		if(x==0) c="0";
		else if(x==1) c="1";
		else if(x==2) c="2";
		else if(x==3) c="3";
		else if(x==4) c="4";
		else if(x==5) c="5";
		else if(x==6) c="6";
		else if(x==7) c="7";
		else if(x==8) c="8";
		else if(x==9) c="9";
		else c=0;
		tmp=tmp-tmp%10;
		tmp=tmp/10;
		strcat(name,c);
		counter--;
	}
	return name;
}


