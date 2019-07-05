#pragma once

#include <string>
#include <map>
#include <optional>
#include <functional>
#include "utilities/Exceptions.h"
#include "utilities/Object.h"
#include "utilities/AST.h"
#include "utilities/EnvironmentHolder.h"

namespace interpreter {

	class Evaluator {
	public:
		static Evaluator* getInstance();

		//generic Evaluate
		std::optional<Value> Evaluate(ASTnode* node);
		
	private:

		std::map<std::string, std::optional<Value>(Evaluator::*)(ASTnode*)> EvaluateDispatcher;
		std::map<std::string, std::optional<Value>(Evaluator::*)(ASTnode*)> IntializeDispatcher();
		
		std::optional<std::reference_wrapper<Value>> Evaluator::EvaluateLvalue(ASTnode* node, bool insertFalg = true, Environment* env = EnvironmentHolder::getInstance()->GetCurrentEnv());
		std::map<std::string, std::optional<std::reference_wrapper<Value>>(Evaluator::*)(ASTnode*, bool, Environment*)> EvaluateLvalueDispatcher;
		std::map<std::string, std::optional<std::reference_wrapper<Value>>(Evaluator::*)(ASTnode*, bool, Environment*)> IntializeLvalueDispatcher();

		inline static Evaluator* evaluator = nullptr;
		Value retVal;

		Evaluator();

		// program
		std::optional<Value> EvaluateProgram(ASTnode* node);

		// expr
		std::optional<Value> EvaluateAddExpr(ASTnode* node);
		std::optional<Value> EvaluateSubExpr(ASTnode* node);
		std::optional<Value> EvaluateMulExpr(ASTnode* node);
		std::optional<Value> EvaluateDivExpr(ASTnode* node);
		std::optional<Value> EvaluateModExpr(ASTnode* node);
		std::optional<Value> EvaluateGreaterExpr(ASTnode* node);
		std::optional<Value> EvaluateGreaterOrEqualExpr(ASTnode* node);
		std::optional<Value> EvaluateLessExpr(ASTnode* node);
		std::optional<Value> EvaluateLessOrEqualExpr(ASTnode* node);
		std::optional<Value> EvaluateEqualExpr(ASTnode* node);
		std::optional<Value> EvaluateNotEqualExpr(ASTnode* node);
		std::optional<Value> EvaluateAndExpr(ASTnode* node);
		std::optional<Value> EvaluateOrExpr(ASTnode* node);


		// term
		std::optional<Value> EvaluateParenthesis(ASTnode* node);
		std::optional<Value> EvaluateUminus(ASTnode* node);
		std::optional<Value> EvaluateNot(ASTnode* node);
		std::optional<Value> EvaluatePreIncrement(ASTnode* node);
		std::optional<Value> EvaluatePostIncrement(ASTnode* node);
		std::optional<Value> EvaluatePreDecrement(ASTnode* node);
		std::optional<Value> EvaluatePostDecrement(ASTnode* node);

		// primary
		std::optional<Value> EvaluateParenthesisFuncdef(ASTnode* node);

		//lvalue
		std::optional<std::reference_wrapper<Value>> EvaluateLvalueIdent(ASTnode* node, bool insertFalg = true, Environment* env = EnvironmentHolder::getInstance()->GetCurrentEnv());
		std::optional<std::reference_wrapper<Value>> EvaluateLvalueLocalIdent(ASTnode* node, bool insertFalg = true, Environment* env = EnvironmentHolder::getInstance()->GetCurrentEnv());
		std::optional<std::reference_wrapper<Value>> EvaluateLvalueGlobalIdent(ASTnode* node, bool insertFalg = true, Environment* env = EnvironmentHolder::getInstance()->GetGlobalEnv());
		
		//rvalue
		std::optional<Value> EvaluateIdent(ASTnode* node);
		std::optional<Value> EvaluateLocalIdent(ASTnode* node);
		std::optional<Value> EvaluateGlobalIdent(ASTnode* node);

		//member
		std::optional<Value> EvaluateMemberLvalueIdent(ASTnode* node);
		std::optional<Value> EvaluateMemberLvalueBrackets(ASTnode* node);
		std::optional<Value> EvaluateMemberCallIdent(ASTnode* node);
		std::optional<Value> EvaluateMemberCallBrackets(ASTnode* node);

		//call
		std::optional<Value> EvaluateMultiCall(ASTnode* node);
		std::optional<Value> EvaluateLvalueCallSuffix(ASTnode* node);
		std::optional<Value> EvaluateFuncdefCall(ASTnode* node);

		std::optional<Value> EvaluateNormCall(ASTnode* node);
		std::optional<Value> EvaluateMethodCall(ASTnode* node);

		//arg
		std::optional<Value> EvaluateArg(ASTnode* node);

		//arglist
		std::optional<Value> EvaluateArglist(ASTnode* node);
		std::optional<Value> EvaluateEmptyArglist(ASTnode* node);

		// assignment
		std::optional<Value> EvaluateAssignExpr(ASTnode* node);

		//stmt
		std::optional<Value> EvaluateIfStmt(ASTnode* node);
		std::optional<Value> EvaluateIfElseStmt(ASTnode* node);
		std::optional<Value> EvaluateWhileStmt(ASTnode* node);
		std::optional<Value> EvaluateForStmt(ASTnode* node);
		std::optional<Value> EvaluateReturnStmt(ASTnode* node);
		std::optional<Value> EvaluateReturnValueStmt(ASTnode* node);
		std::optional<Value> EvaluateBreak(ASTnode* node);
		std::optional<Value> EvaluateContinue(ASTnode* node);
		std::optional<Value> EvaluateSemicolon(ASTnode* node);

		//elist
		std::optional<Value> EvaluateElist(ASTnode* node);
		std::optional<Value> EvaluateEmptyElist(ASTnode* node);

		//indexed
		std::optional<Value> EvaluateIndexed(ASTnode* node);

		//indexedElem
		std::optional<Value> EvaluateIndexedElem(ASTnode* node);

		//object
		std::optional<Value> EvaluateElistObjectdef(ASTnode* node);
		std::optional<Value> EvaluateIndexedObjectdef(ASTnode* node);

		//block
		std::optional<Value> EvaluateBlock(ASTnode* node);

		//funcdef
		std::optional<Value> EvaluateFuncdef(ASTnode* node);
		std::optional<Value> EvaluateAnonymousFuncdef(ASTnode* node);

		// const
		std::optional<Value> EvaluateNumberConst(ASTnode* node);
		std::optional<Value> EvaluateStringConst(ASTnode* node);
		std::optional<Value> EvaluateBoolConst(ASTnode* node);
		std::optional<Value> EvaluateNIL(ASTnode* node);

		//formal
		std::optional<Value> EvaluateParam(ASTnode* node);
		std::optional<Value> EvaluateOptionalParam(ASTnode* node);

		//idlist
		std::optional<Value> EvaluateIdlist(ASTnode* node);
		std::optional<Value> EvaluateEmptyIdlist(ASTnode* node);

	};

}