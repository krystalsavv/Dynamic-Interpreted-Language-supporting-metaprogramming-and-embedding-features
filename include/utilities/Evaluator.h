#pragma once

#include <string>
#include "utilities/Object.h"

class Evaluator
{
public:
	Evaluator();
	//~Evaluator();
	Value* EvaluateVar(ASTnode* node);
	Value* EvaluateAddExpr(ASTnode* node);
	Value* EvaluateIfStmt(ASTnode* node);

	//generic evaluate 
	Value* Evaluate(ASTnode* node);
private:

};

