#pragma once

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include "parser.hpp"
#include "scanner.h"
#include "..//utilities/Object.h"
#include "..//utilities/AST.h"
#include "..//utilities/EnvironmentHolder.h"
#include "..//utilities/Evaluator.h"

namespace interpreter {

	class Interpreter
	{
		void TerminateInterpreterNonInteractive(AST* ast, yyscan_t scanner);
		void TerminateLoopInteractive(AST* ast, yyscan_t scanner);
		std::string InputToString(const char* filename);
	
	public:
		Interpreter() = default;
		void  InvokeInterpreter(char* file);
	};

}



