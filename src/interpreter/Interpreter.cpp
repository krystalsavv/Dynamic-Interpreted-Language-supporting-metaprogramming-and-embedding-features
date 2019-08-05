#include "interpreter/Interpreter.h"

using namespace interpreter;

size_t interpreter::lineNumber = 1;

void  Interpreter::InvokeInterpreter( char* file) {
	if (file != NULL) {
		yyscan_t scanner;
		InitializeNonInteractive(scanner,file);
		AST* ast = new AST();
		Parse(ast, scanner);
		Evaluate(ast);
		TerminateInterpreterNonInteractive(ast, scanner);
	}
	else {
		std::string inputString = "";
		InitGlobalEnvironment();
		while (true) {
			yyscan_t scanner;
			InitializeEveryLoopInteractive(scanner, inputString);
			AST* ast = new AST();
			if (!ParseInteractive(ast, scanner))	continue;
			if (!EvaluateInteractive(ast,scanner)) continue;
			TerminateLoopInteractive(ast, scanner);
		}
	}
}


void Interpreter::InitializeNonInteractive(yyscan_t& scanner, char* file) {
	yylex_init(&scanner);
	std::string inputString = InputToString(file);
	yy_scan_string(inputString.c_str(), scanner);
	yyset_lineno(1, scanner);
	InitGlobalEnvironment();
}

void Interpreter::Parse(AST* ast, yyscan_t& scanner) {
	try {
		yyparse(ast, scanner, 0);
	}
	catch (BreakException& e) { std::cout << std::endl << e.what() << std::endl; exit(0); }
	catch (ContinueException& e) { std::cout << std::endl << e.what() << std::endl; exit(0); }
	catch (ReturnException& e) { std::cout << std::endl << e.what() << std::endl; exit(0); }
	catch (ReturnValueException& e) { std::cout << std::endl << e.what() << std::endl; exit(0); }
	catch (RuntimeErrorException& e) { std::cout << std::endl << e.what() << std::endl; exit(0); }
	catch (SyntaxErrorException& e) { std::cout << std::endl << e.what() << std::endl; exit(0); }
}

void Interpreter::Evaluate(AST* ast) {
	try {
		*Evaluator::getInstance()->Evaluate(ast->GetRoot());
	}
	catch (BreakException& e) { std::cout << std::endl << e.what() << std::endl; exit(0); }
	catch (ContinueException& e) { std::cout << std::endl << e.what() << std::endl; exit(0); }
	catch (ReturnException& e) { std::cout << std::endl << e.what() << std::endl; exit(0); }
	catch (ReturnValueException& e) { std::cout << std::endl << e.what() << std::endl; exit(0); }
	catch (RuntimeErrorException& e) { std::cout << std::endl << e.what() << std::endl; exit(0); }
	catch (SyntaxErrorException& e) { std::cout << std::endl << e.what() << std::endl; exit(0); }
}

void  Interpreter::TerminateInterpreterNonInteractive(AST* ast, yyscan_t& scanner) {
	yylex_destroy(scanner);
	TraverseAndClearAst(ast->GetRoot());
	ClearEnvironment();
	DestroyAst(ast->GetRoot());
	delete ast;
	EnvironmentHolder::destroyInstance();
	Evaluator::destroyInstance();
	SyntaxParser::destroyInstance();
	MetaUnparser::destroyInstance();
}


void Interpreter::InitializeEveryLoopInteractive(yyscan_t& scanner, std::string& inputString) {
	yylex_init(&scanner);
	std::cout << "\nAlphaDI > ";
	getline(std::cin, inputString);
	yy_scan_string(inputString.c_str(), scanner);
	yyset_lineno(1, scanner);
}

bool Interpreter::ParseInteractive(AST* ast, yyscan_t& scanner) {
	try{
		yyparse(ast, scanner, 1);
		return 1;
	}
	catch (BreakException& e) { std::cout << std::endl << e.what() << std::endl; TerminateLoopInteractive(ast, scanner); return 0; }
	catch (ContinueException& e) { std::cout << std::endl << e.what() << std::endl; TerminateLoopInteractive(ast, scanner); return 0; }
	catch (ReturnException& e) { std::cout << std::endl << e.what() << std::endl; TerminateLoopInteractive(ast, scanner); return 0; }
	catch (ReturnValueException& e) { std::cout << std::endl << e.what() << std::endl; TerminateLoopInteractive(ast, scanner); return 0; }
	catch (RuntimeErrorException& e) { std::cout << std::endl << e.what() << std::endl; TerminateLoopInteractive(ast, scanner); return 0; }
	catch (SyntaxErrorException& e) { std::cout << std::endl << e.what() << std::endl; TerminateLoopInteractive(ast, scanner); return 0; }
}

bool Interpreter::EvaluateInteractive(AST* ast, yyscan_t& scanner) {
	try {
		*Evaluator::getInstance()->Evaluate(ast->GetRoot());
		return 1;
	}
	catch (BreakException& e) { std::cout << std::endl << e.what() << std::endl; TerminateLoopInteractive(ast, scanner); return 0; }
	catch (ContinueException& e) { std::cout << std::endl << e.what() << std::endl; TerminateLoopInteractive(ast, scanner); return 0; }
	catch (ReturnException& e) { std::cout << std::endl << e.what() << std::endl; TerminateLoopInteractive(ast, scanner); return 0; }
	catch (ReturnValueException& e) { std::cout << std::endl << e.what() << std::endl; TerminateLoopInteractive(ast, scanner); return 0; }
	catch (RuntimeErrorException& e) { std::cout << std::endl << e.what() << std::endl; TerminateLoopInteractive(ast, scanner); return 0; }
	catch (SyntaxErrorException& e) { std::cout << std::endl << e.what() << std::endl; TerminateLoopInteractive(ast, scanner); return 0; }
}

void Interpreter::TerminateLoopInteractive(AST* ast, yyscan_t& scanner) {
	yylex_destroy(scanner);
	if (ast->GetRoot())
		ast->GetRoot()->DecreaseReferenceCounter();
}

std::string  Interpreter::InputToString(const char* filename) {
	std::ifstream ifs(filename);
	std::string str((std::istreambuf_iterator<char>(ifs)), std::istreambuf_iterator<char>());

	return str;
}

