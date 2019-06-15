#pragma once

#include <string>
#include <map>
#include "utilities/Object.h"

class Evaluator
{
public:
	Evaluator() = default;
	Value Evaluate(ASTnode* node);
private:
	static std::map<std::string, Value (Evaluator::*)(ASTnode*) > EvaluateDispatcher;
	Value EvaluateAddExpr(ASTnode* node);
};

