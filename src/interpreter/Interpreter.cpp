#include "interpreter/Interpreter.h"

using namespace interpreter;

void  Interpreter::InvokeInterpreter( char* file) {
	if (file != NULL) {
		yyscan_t scanner;
		yylex_init(&scanner);
		std::string inputString = InputToString(file);
		yy_scan_string(inputString.c_str(), scanner);
		yyset_lineno(1, scanner);
		AST* ast = new AST();
		try { yyparse(ast, scanner, 0);	InitGlobalEnvironment(); *Evaluator::getInstance()->Evaluate(ast->GetRoot()); }
		catch (BreakException& e) { std::cout << std::endl << e.what() << " at line " << yyget_lineno(scanner) << std::endl; exit(0); }
		catch (ContinueException& e) { std::cout << std::endl << e.what() << " at line " << yyget_lineno(scanner) << std::endl; exit(0); }
		catch (ReturnException& e) { std::cout << std::endl << e.what() << " at line " << yyget_lineno(scanner) << std::endl; exit(0); }
		catch (ReturnValueException& e) { std::cout << std::endl << e.what() << " at line " << yyget_lineno(scanner) << std::endl; exit(0); }
		catch (RuntimeErrorException& e) { std::cout << std::endl << e.what() << " at line " << yyget_lineno(scanner) << std::endl; exit(0); }
		catch (SyntaxErrorException& e) { std::cout << std::endl << e.what() << " at line " << yyget_lineno(scanner) << std::endl; exit(0); }
		//ast->Print();
		//std::cout << "------------------------------ EnvironmentChain -----------------------------------" << std::endl << std::endl;
		//EnvironmentHolder::getInstance()->PrintEnvironmentChain();
		//std::cout << "------------------------------ GlobalEnvironment -----------------------------------" << std::endl << std::endl;
		//EnvironmentHolder::getInstance()->PrintGlobalEnvironment();
		TerminateInterpreterNonInteractive(ast, scanner);
	}
	else {
		std::string inputString = "";
		InitGlobalEnvironment();
		while (true) {
			yyscan_t scanner;
			yylex_init(&scanner);
			std::cout << "\nAlphaDI > ";
			getline(std::cin, inputString);
			yy_scan_string(inputString.c_str(), scanner);
			yyset_lineno(1, scanner);
			AST* ast = new AST();
			try { yyparse(ast, scanner, 1);  *Evaluator::getInstance()->Evaluate(ast->GetRoot()); }
			catch (BreakException& e) { std::cout << std::endl << e.what() << " at line " << yyget_lineno(scanner) << std::endl; TerminateLoopInteractive(ast, scanner); continue; }
			catch (ContinueException& e) { std::cout << std::endl << e.what() << " at line " << yyget_lineno(scanner) << std::endl; TerminateLoopInteractive(ast, scanner); continue; }
			catch (ReturnException& e) { std::cout << std::endl << e.what() << " at line " << yyget_lineno(scanner) << std::endl; TerminateLoopInteractive(ast, scanner); continue; }
			catch (ReturnValueException& e) { std::cout << std::endl << e.what() << " at line " << yyget_lineno(scanner) << std::endl; TerminateLoopInteractive(ast, scanner); continue; }
			catch (RuntimeErrorException& e) { std::cout << std::endl << e.what() << " at line " << yyget_lineno(scanner) << std::endl; TerminateLoopInteractive(ast, scanner); continue; }
			catch (SyntaxErrorException& e) { std::cout << std::endl << e.what() << " at line " << yyget_lineno(scanner) << std::endl; TerminateLoopInteractive(ast, scanner); continue; }
			TerminateLoopInteractive(ast, scanner);

		}
		//ast->Print();
		//std::cout << "------------------------------ EnvironmentChain -----------------------------------" << std::endl << std::endl;
		//EnvironmentHolder::getInstance()->PrintEnvironmentChain();
		//std::cout << "------------------------------ GlobalEnvironment -----------------------------------" << std::endl << std::endl;
		//EnvironmentHolder::getInstance()->PrintGlobalEnvironment();
	}
}

void  Interpreter::TerminateInterpreterNonInteractive(AST* ast, yyscan_t scanner) {
	yylex_destroy(scanner);
	TraverseAndClearAst(ast->GetRoot());
	ClearEnvironment();
	DestroyAst(ast->GetRoot());
	delete ast;
	EnvironmentHolder::getInstance()->destroyInstance();
	Evaluator::getInstance()->destroyInstance();
}

void Interpreter::TerminateLoopInteractive(AST* ast, yyscan_t scanner) {
	yylex_destroy(scanner);
	if (ast->GetRoot())
		ast->GetRoot()->DecreaseReferenceCounter();
}

std::string  Interpreter::InputToString(const char* filename) {
	std::ifstream ifs(filename);
	std::string str((std::istreambuf_iterator<char>(ifs)), std::istreambuf_iterator<char>());

	return str;
}

