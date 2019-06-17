#pragma once

#include <string>
#include <map>
#include "utilities/Object.h"

class Evaluator {
public:
	Evaluator() = default;
	Value Evaluate(ASTnode* node);
private:
	static std::map<std::string, Value (Evaluator::*)(ASTnode*)> EvaluateDispatcher;
	static std::map<std::string, Value(Evaluator::*)(ASTnode*)> IntializeDispatcher();
	
	// term
	Value EvaluateParenthesis(ASTnode* node);
	Value EvaluateUminus(ASTnode* node);
	Value EvaluateNot(ASTnode* node);
	//Value EvaluatePreIncrement(ASTnode* node);
	//Value EvaluatePostIncrement(ASTnode* node);
	//Value EvaluatePreDecrement(ASTnode* node);
	//Value EvaluatePostDecrement(ASTnode* node);

	// primary
	//Value EvaluateParenthesisFuncdef(ASTnode* node);

	// const
	Value EvaluateNumberConst(ASTnode* node);
	Value EvaluateStringConst(ASTnode* node);
	Value EvaluateBoolConst(ASTnode* node);
	//Value EvaluateNIL(ASTnode* node);

	// expr
	Value EvaluateAddExpr(ASTnode* node);
	Value EvaluateSubExpr(ASTnode* node);
	Value EvaluateMulExpr(ASTnode* node);
	Value EvaluateDivExpr(ASTnode* node);
	Value EvaluateModExpr(ASTnode* node);
	Value EvaluateGreaterExpr(ASTnode* node);
	Value EvaluateGreaterOrEqualExpr(ASTnode* node);
	Value EvaluateLessExpr(ASTnode* node);
	Value EvaluateLessOrEqualExpr(ASTnode* node);
	Value EvaluateEqualExpr(ASTnode* node);
	Value EvaluateNotEqualExpr(ASTnode* node);
	Value EvaluateAndExpr(ASTnode* node);
	Value EvaluateOrExpr(ASTnode* node);

	//stmt
	/*Value EvaluateIfStmt(ASTnode* node);
	Value EvaluateWhileStmt(ASTnode* node);
	Value EvaluateForStmt(ASTnode* node);
	Value EvaluateReturnStmt(ASTnode* node);*/
	//Value EvaluateBreak(ASTnode* node);
	//Value EvaluateContinue(ASTnode* node);
	//Value EvaluateSemicolon(ASTnode* node);


};

