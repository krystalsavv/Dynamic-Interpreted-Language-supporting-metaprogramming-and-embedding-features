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
#include "..//utilities/MetaParser.h"

namespace interpreter {

	class Interpreter
	{

		void InitializeNonInteractive(yyscan_t& scanner, char* file);
		void Parse(AST* ast, yyscan_t& scanner);
		void Evaluate(AST* ast);
		void TerminateInterpreterNonInteractive(AST* ast, yyscan_t& scanner);

		void InitializeEveryLoopInteractive(yyscan_t& scanner, std::string inputString);
		bool ParseInteractive(AST* ast, yyscan_t& scanner);
		bool Interpreter::EvaluateInteractive(AST* ast, yyscan_t& scanner);
		void TerminateLoopInteractive(AST* ast, yyscan_t& scanner);

		std::string InputToString(const char* filename);

	public:
		Interpreter() = default;
		void  InvokeInterpreter(char* file);

		void Initialize();
		void AddVariable(std::string id, Value value);
		void Execute(AST* ast, yyscan_t& scanner, std::string inputString);
	};

}



