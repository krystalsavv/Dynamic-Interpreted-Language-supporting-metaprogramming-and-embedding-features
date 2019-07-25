#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
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

void TerminateInterpreter(AST* ast, yyscan_t scanner) {
	yylex_destroy(scanner);
	TraverseAndClearAst(ast->GetRoot());
	ClearEnvironment();
	DestroyAst(ast->GetRoot());
	delete ast;
	EnvironmentHolder::getInstance()->destroyInstance();
	Evaluator::getInstance()->destroyInstance();
}



int main(int argc,char** argv){
	yyscan_t scanner;
	yylex_init(&scanner);

	if(argc>1) {
		std::string inputString = InputToString(argv[1]);
		yy_scan_string(inputString.c_str(), scanner);
		AST* ast = new AST();
		try { yyparse(ast, scanner, 0); }
		catch (RuntimeErrorException& e) { std::cout << std::endl << e.what() << std::endl; exit(0); }
		catch (SyntaxErrorException& e) { std::cout << std::endl << e.what() << std::endl; exit(0); }
		*Evaluator::getInstance()->Evaluate(ast->GetRoot());
		//ast->Print();
		//std::cout << "------------------------------ EnvironmentChain -----------------------------------" << std::endl << std::endl;
		//EnvironmentHolder::getInstance()->PrintEnvironmentChain();
		//std::cout << "------------------------------ GlobalEnvironment -----------------------------------" << std::endl << std::endl;
		//EnvironmentHolder::getInstance()->PrintGlobalEnvironment();
		TerminateInterpreter(ast, scanner);
	} else {
		std::string inputString = "";
		//yyset_in(stdin, scanner);
		while (true) {
			std::cout << "\nAlphaDI > ";
			getline(std::cin, inputString);
			yy_scan_string(inputString.c_str(), scanner);
			AST* ast = new AST();
			try { yyparse(ast, scanner, 0); }
			catch (RuntimeErrorException& e) { std::cout << std::endl << e.what() << std::endl; exit(0); }
			catch (SyntaxErrorException& e) { std::cout << std::endl << e.what() << std::endl; exit(0); }
			*Evaluator::getInstance()->Evaluate(ast->GetRoot());
		}
		//ast->Print();
		//std::cout << "------------------------------ EnvironmentChain -----------------------------------" << std::endl << std::endl;
		//EnvironmentHolder::getInstance()->PrintEnvironmentChain();
		//std::cout << "------------------------------ GlobalEnvironment -----------------------------------" << std::endl << std::endl;
		//EnvironmentHolder::getInstance()->PrintGlobalEnvironment();
		//TerminateInterpreter(ast, scanner);
	}


	return 0;
}