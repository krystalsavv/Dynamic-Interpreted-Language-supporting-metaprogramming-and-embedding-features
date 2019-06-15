#include "utilities/Evaluator.h"

std::map<std::string, Value(Evaluator::*)(ASTnode*) > IntializeDispatcher() {
	std::map<std::string, Value(Evaluator::*)(ASTnode*) > table;
	table["ADD"] = &Evaluator::EvaluateAddExpr;
	return table;
}

std::map<std::string, Value(Evaluator::*)(ASTnode*) > Evaluator::EvaluateDispatcher = IntializeDispatcher();


Value Evaluator::Evaluate(ASTnode* node) {
	return EvaluateDispatcher[node->GetValue("type").GetStringValue()];
}


Value Evaluator::EvaluateAddExpr(ASTnode* node) {
	Value left = node->GetValue("left");
	Value right = node->GetValue("right");
	return left;								// TODO the sum left + right
}