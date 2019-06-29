#pragma once

#include <string>
#include <map>
#include "utilities/Exceptions.h"
#include "utilities/Object.h"
#include "utilities/AST.h"
#include "utilities/EnvironmentHolder.h"


namespace interpreter {

	class Evaluator {
	public:
		static Evaluator* getInstance();

		//generic Evaluate
		Value Evaluate(ASTnode* node);

	private:
		std::map<std::string, Value(Evaluator::*)(ASTnode*)> EvaluateDispatcher;
		std::map<std::string, Value(Evaluator::*)(ASTnode*)> IntializeDispatcher();
		inline static Evaluator* evaluator = nullptr;
		Value retVal;

		Evaluator();

		// program
		Value EvaluateProgram(ASTnode* node);

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
		Value EvaluateNIL(ASTnode* node);

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

		// assignment
		//Value EvaluateAssignExpr(ASTnode* node);

		//stmt
		Value EvaluateIfStmt(ASTnode* node);
		Value EvaluateIfElseStmt(ASTnode* node);
		Value EvaluateWhileStmt(ASTnode* node);
		Value EvaluateForStmt(ASTnode* node);
		Value EvaluateReturnStmt(ASTnode* node);
		Value EvaluateReturnValueStmt(ASTnode* node);
		Value EvaluateBreak(ASTnode* node);
		Value EvaluateContinue(ASTnode* node);
		Value EvaluateBlock(ASTnode* node);
		Value EvaluateSemicolon(ASTnode* node);

		//elist
		Value EvaluateElist(ASTnode* node);
		Value EvaluateEmptyElist(ASTnode* node);

		//indexed
		Value EvaluateIndexed(ASTnode* node);

		//indexedElem
		Value EvaluateIndexedElem(ASTnode* node);

		//object
		Value EvaluateElistObjectdef(ASTnode* node);
		Value EvaluateIndexedObjectdef(ASTnode* node);

	};

}