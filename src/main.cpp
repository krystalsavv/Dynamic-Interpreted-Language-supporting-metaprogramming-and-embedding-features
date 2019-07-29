#include "interpreter/Interpreter.h"

int main(int argc,char** argv){
	Interpreter inter;
	if (argc > 1) 
		inter.InvokeInterpreter(argv[1]);
	else 
		inter.InvokeInterpreter(NULL);
	return 0;
}