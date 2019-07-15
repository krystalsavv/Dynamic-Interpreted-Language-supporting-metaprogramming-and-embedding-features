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
		std::optional<Value> Evaluate(ASTnode* node, bool insertFlag = true);
		
	private:

		std::map<std::string, std::optional<Value>(Evaluator::*)(ASTnode*, bool)> EvaluateDispatcher;
		std::map<std::string, std::optional<Value>(Evaluator::*)(ASTnode*, bool)> IntializeDispatcher();
		
		std::optional<std::reference_wrapper<Value>> EvaluateLvalue(ASTnode* node, bool insertFlag = true, Environment* env = EnvironmentHolder::getInstance()->GetCurrentEnv());
		std::map<std::string, std::optional<std::reference_wrapper<Value>>(Evaluator::*)(ASTnode*, bool, Environment*)> EvaluateLvalueDispatcher;
		std::map<std::string, std::optional<std::reference_wrapper<Value>>(Evaluator::*)(ASTnode*, bool, Environment*)> IntializeLvalueDispatcher();

		inline static Evaluator* evaluator = nullptr;
		Value retVal;

		Evaluator();

		// program
		std::optional<Value> EvaluateProgram(ASTnode* node, bool insertFlag = true);

		// expr
		std::optional<Value> EvaluateAddExpr(ASTnode* node, bool insertFlag = true);
		std::optional<Value> EvaluateSubExpr(ASTnode* node, bool insertFlag = true);
		std::optional<Value> EvaluateMulExpr(ASTnode* node, bool insertFlag = true);
		std::optional<Value> EvaluateDivExpr(ASTnode* node, bool insertFlag = true);
		std::optional<Value> EvaluateModExpr(ASTnode* node, bool insertFlag = true);
		std::optional<Value> EvaluateGreaterExpr(ASTnode* node, bool insertFlag = true);
		std::optional<Value> EvaluateGreaterOrEqualExpr(ASTnode* node, bool insertFlag = true);
		std::optional<Value> EvaluateLessExpr(ASTnode* node, bool insertFlag = true);
		std::optional<Value> EvaluateLessOrEqualExpr(ASTnode* node, bool insertFlag = true);
		std::optional<Value> EvaluateEqualExpr(ASTnode* node, bool insertFlag = true);
		std::optional<Value> EvaluateNotEqualExpr(ASTnode* node, bool insertFlag = true);
		std::optional<Value> EvaluateAndExpr(ASTnode* node, bool insertFlag = true);
		std::optional<Value> EvaluateOrExpr(ASTnode* node, bool insertFlag = true);


		// term
		std::optional<Value> EvaluateParenthesis(ASTnode* node, bool insertFlag = true);
		std::optional<Value> EvaluateUminus(ASTnode* node, bool insertFlag = true);
		std::optional<Value> EvaluateNot(ASTnode* node, bool insertFlag = true);
		std::optional<Value> EvaluatePreIncrement(ASTnode* node, bool insertFlag = true);
		std::optional<Value> EvaluatePostIncrement(ASTnode* node, bool insertFlag = true);
		std::optional<Value> EvaluatePreDecrement(ASTnode* node, bool insertFlag = true);
		std::optional<Value> EvaluatePostDecrement(ASTnode* node, bool insertFlag = true);

		// primary
		std::optional<Value> EvaluateParenthesisFuncdef(ASTnode* node, bool insertFlag = true);

		//lvalue
		std::optional<std::reference_wrapper<Value>> EvaluateLvalueIdent(ASTnode* node, bool insertFlag = true, Environment* env = EnvironmentHolder::getInstance()->GetCurrentEnv());
		std::optional<std::reference_wrapper<Value>> EvaluateLvalueLocalIdent(ASTnode* node, bool insertFlag = true, Environment* env = EnvironmentHolder::getInstance()->GetCurrentEnv());
		std::optional<std::reference_wrapper<Value>> EvaluateLvalueGlobalIdent(ASTnode* node, bool insertFlag = true, Environment* env = EnvironmentHolder::getInstance()->GetGlobalEnv());
		
		//rvalue
		std::optional<Value> EvaluateIdent(ASTnode* node, bool insertFlag = true);
		std::optional<Value> EvaluateLocalIdent(ASTnode* node, bool insertFlag = true);
		std::optional<Value> EvaluateGlobalIdent(ASTnode* node, bool insertFlag = true);

		//lvalue member
		std::optional<std::reference_wrapper<Value>> EvaluateLvalueMemberIdent(ASTnode* node, bool insertFlag = true, Environment* env = EnvironmentHolder::getInstance()->GetCurrentEnv());
		std::optional<std::reference_wrapper<Value>> EvaluateLvalueMemberBrackets(ASTnode* node, bool insertFlag = true, Environment* env = EnvironmentHolder::getInstance()->GetCurrentEnv());
		std::optional<std::reference_wrapper<Value>> EvaluateLvalueMemberCallIdent(ASTnode* node, bool insertFlag = true, Environment* env = EnvironmentHolder::getInstance()->GetCurrentEnv());
		std::optional<std::reference_wrapper<Value>> EvaluateLvalueMemberCallBrackets(ASTnode* node, bool insertFlag = true, Environment* env = EnvironmentHolder::getInstance()->GetCurrentEnv());

		//member
		std::optional<Value> EvaluateMemberIdent(ASTnode* node, bool insertFlag = true);
		std::optional<Value> EvaluateMemberBrackets(ASTnode* node, bool insertFlag = true);
		std::optional<Value> EvaluateMemberCallIdent(ASTnode* node, bool insertFlag = true);
		std::optional<Value> EvaluateMemberCallBrackets(ASTnode* node, bool insertFlag = true);

		//call
		std::optional<Value> EvaluateMultiCall(ASTnode* node, bool insertFlag = true);
		std::optional<Value> EvaluateLvalueCallSuffix(ASTnode* node, bool insertFlag = true);
		std::optional<Value> EvaluateFuncdefCall(ASTnode* node, bool insertFlag = true);

		std::optional<Value> EvaluateNormCall(ASTnode* node, bool insertFlag = true);
		std::optional<Value> EvaluateMethodCall(ASTnode* node, bool insertFlag = true);

		//arg
		std::optional<Value> EvaluateArg(ASTnode* node, bool insertFlag = true);

		//arglist
		std::optional<Value> EvaluateArglist(ASTnode* node, bool insertFlag = true);
		std::optional<Value> EvaluateEmptyArglist(ASTnode* node, bool insertFlag = true);

		// assignment
		std::optional<Value> EvaluateAssignExpr(ASTnode* node, bool insertFlag = true);

		//stmt
		std::optional<Value> EvaluateIfStmt(ASTnode* node, bool insertFlag = true);
		std::optional<Value> EvaluateIfElseStmt(ASTnode* node, bool insertFlag = true);
		std::optional<Value> EvaluateWhileStmt(ASTnode* node, bool insertFlag = true);
		std::optional<Value> EvaluateForStmt(ASTnode* node, bool insertFlag = true);
		std::optional<Value> EvaluateReturnStmt(ASTnode* node, bool insertFlag = true);
		std::optional<Value> EvaluateReturnValueStmt(ASTnode* node, bool insertFlag = true);
		std::optional<Value> EvaluateBreak(ASTnode* node, bool insertFlag = true);
		std::optional<Value> EvaluateContinue(ASTnode* node, bool insertFlag = true);
		std::optional<Value> EvaluateSemicolon(ASTnode* node, bool insertFlag = true);

		//elist
		std::optional<Value> EvaluateElist(ASTnode* node, bool insertFlag = true);
		std::optional<Value> EvaluateEmptyElist(ASTnode* node, bool insertFlag = true);

		//indexed
		std::optional<Value> EvaluateIndexed(ASTnode* node, bool insertFlag = true);

		//indexedElem
		std::optional<Value> EvaluateIndexedElem(ASTnode* node, bool insertFlag = true);

		//object
		std::optional<Value> EvaluateElistObjectdef(ASTnode* node, bool insertFlag = true);
		std::optional<Value> EvaluateIndexedObjectdef(ASTnode* node, bool insertFlag = true);

		//block
		std::optional<Value> EvaluateBlock(ASTnode* node, bool insertFlag = true);

		//funcdef
		std::optional<Value> EvaluateFuncdef(ASTnode* node, bool insertFlag = true);
		std::optional<Value> EvaluateAnonymousFuncdef(ASTnode* node, bool insertFlag = true);
		std::optional<Value> EvaluateFuncEnter(ASTnode* node, bool insertFlag = true);
		std::optional<Value> EvaluateFuncBody(ASTnode* node, bool insertFlag);

		// const
		std::optional<Value> EvaluateNumberConst(ASTnode* node, bool insertFlag = true);
		std::optional<Value> EvaluateStringConst(ASTnode* node, bool insertFlag = true);
		std::optional<Value> EvaluateBoolConst(ASTnode* node, bool insertFlag = true);
		std::optional<Value> EvaluateNIL(ASTnode* node, bool insertFlag = true);

		//formal
		std::optional<Value> EvaluateParam(ASTnode* node, bool insertFlag = true);
		std::optional<Value> EvaluateOptionalParam(ASTnode* node, bool insertFlag = true);

		//idlist
		std::optional<Value> EvaluateIdlist(ASTnode* node, bool insertFlag = true);
		std::optional<Value> EvaluateEmptyIdlist(ASTnode* node, bool insertFlag = true);

		//libFunctions
		std::optional<Value> EvaluatePrint(ASTnode* node, bool insertFlag = true);
		std::optional<Value> EvaluateTypeof(ASTnode* node, bool insertFlag = true);
		std::optional<Value> EvaluateObject_keys(ASTnode* node, bool insertFlag = true);
		std::optional<Value> EvaluateObject_size(ASTnode* node, bool insertFlag = true);
		std::optional<Value> EvaluateEval(ASTnode* node, bool insertFlag = true);

		
	};

}