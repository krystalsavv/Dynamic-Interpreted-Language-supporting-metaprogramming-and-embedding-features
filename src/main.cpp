#include <string>
#include <iostream>
#include <fstream>
#include "parser.hpp"
#include "scanner.h"
#include "utilities/Object.h"
#include "utilities/AST.h"
#include "utilities/EnvironmentHolder.h"
#include "utilities/Evaluator.h"

std::string InputToString(const char* filename) {
	std::ifstream ifs(filename);
	std::string str((std::istreambuf_iterator<char>(ifs)), std::istreambuf_iterator<char>());

	return str;
}



int main(int argc,char** argv){
	yyscan_t scanner;
	yylex_init(&scanner);

	if(argc>1) {
		std::string inputString = InputToString(argv[1]);
		yy_scan_string(inputString.c_str(), scanner);
		// if(argc==3){
		//	FILE * output = fopen(argv[2], "w");
	 	// 	yyset_out(output, scanner);
	 	// }
	} else {
		yyset_in(stdin, scanner);
	}
	AST* ast = new AST();
	try { yyparse(ast, scanner, 0); }
	catch (RuntimeErrorException& e) { std::cout << std::endl << e.what() << std::endl; exit(0); }
	catch (SyntaxErrorException& e) { std::cout << std::endl << e.what() << std::endl; exit(0); }
	yylex_destroy(scanner);
	//ast->Print();
	*Evaluator::getInstance()->Evaluate(ast->GetRoot());
	std::cout << "------------------------------ EnvironmentChain -----------------------------------" << std::endl << std::endl;
	EnvironmentHolder::getInstance()->PrintEnvironmentChain();
	//std::cout << "------------------------------ GlobalEnvironment -----------------------------------" << std::endl << std::endl;
	//EnvironmentHolder::getInstance()->PrintGlobalEnvironment();
	//TraverseAndClearAst(ast->GetRoot());
	//std::cout << "------------------------------ EnvironmentChain -----------------------------------" << std::endl << std::endl;
	//EnvironmentHolder::getInstance()->PrintEnvironmentChain();
	//ClearEnvironment();
	//DestroyAst(ast->GetRoot());
	//delete ast;
	//EnvironmentHolder::getInstance()->destroyInstance();
	//Evaluator::getInstance()->destroyInstance();

	return 0;
}