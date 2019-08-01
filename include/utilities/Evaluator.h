#pragma once
#include <string>
#include <map>
#include <optional>
#include <functional>
#include "utilities/Exceptions.h"
#include "utilities/Object.h"
#include "utilities/AST.h"
#include "utilities/EvaluatorActions.h"

#define OPValue  std::optional<Value>

namespace interpreter {
	extern ASTnode* SyntaxParseOuter(ASTnode* node);

	class Evaluator {
	public:
		static Evaluator* getInstance();
		static void destroyInstance();
		//generic Evaluate
		OPValue Evaluate(ASTnode* node, bool insertFlag = true);

	private:
		std::map<std::string, OPValue (Evaluator::*)(ASTnode*, bool)> EvaluateDispatcher;
		std::map<std::string, OPValue (Evaluator::*)(ASTnode*, bool)> IntializeDispatcher();
		
		Value& EvaluateLvalue(ASTnode* node, bool insertFlag = true, Environment* env = EnvironmentHolder::getInstance()->GetCurrentEnv());
		std::map<std::string, Value& (Evaluator::*)(ASTnode*, bool, Environment*)> EvaluateLvalueDispatcher;
		std::map<std::string, Value& (Evaluator::*)(ASTnode*, bool, Environment*)> IntializeLvalueDispatcher();

		inline static Evaluator* evaluator = nullptr;
		Value retVal;
		bool retFlag = false;
		Evaluator();

		// program
		OPValue EvaluateProgram(ASTnode* node, bool insertFlag = true);

		// expr
		OPValue EvaluateAddExpr(ASTnode* node, bool insertFlag = true);
		OPValue EvaluateSubExpr(ASTnode* node, bool insertFlag = true);
		OPValue EvaluateMulExpr(ASTnode* node, bool insertFlag = true);
		OPValue EvaluateDivExpr(ASTnode* node, bool insertFlag = true);
		OPValue EvaluateModExpr(ASTnode* node, bool insertFlag = true);
		OPValue EvaluateGreaterExpr(ASTnode* node, bool insertFlag = true);
		OPValue EvaluateGreaterOrEqualExpr(ASTnode* node, bool insertFlag = true);
		OPValue EvaluateLessExpr(ASTnode* node, bool insertFlag = true);
		OPValue EvaluateLessOrEqualExpr(ASTnode* node, bool insertFlag = true);
		OPValue EvaluateEqualExpr(ASTnode* node, bool insertFlag = true);
		OPValue EvaluateNotEqualExpr(ASTnode* node, bool insertFlag = true);
		OPValue EvaluateAndExpr(ASTnode* node, bool insertFlag = true);
		OPValue EvaluateOrExpr(ASTnode* node, bool insertFlag = true);


		// term
		OPValue EvaluateParenthesis(ASTnode* node, bool insertFlag = true);
		OPValue EvaluateUminus(ASTnode* node, bool insertFlag = true);
		OPValue EvaluateNot(ASTnode* node, bool insertFlag = true);
		OPValue EvaluatePreIncrement(ASTnode* node, bool insertFlag = true);
		OPValue EvaluatePostIncrement(ASTnode* node, bool insertFlag = true);
		OPValue EvaluatePreDecrement(ASTnode* node, bool insertFlag = true);
		OPValue EvaluatePostDecrement(ASTnode* node, bool insertFlag = true);

		// primary
		OPValue EvaluateParenthesisFuncdef(ASTnode* node, bool insertFlag = true);

		//lvalue
		Value& EvaluateLvalueIdent(ASTnode* node, bool insertFlag = true, Environment* env = EnvironmentHolder::getInstance()->GetCurrentEnv());
		Value& EvaluateLvalueLocalIdent(ASTnode* node, bool insertFlag = true, Environment* env = EnvironmentHolder::getInstance()->GetCurrentEnv());
		Value& EvaluateLvalueGlobalIdent(ASTnode* node, bool insertFlag = true, Environment* env = EnvironmentHolder::getInstance()->GetGlobalEnv());
		
		//rvalue
		OPValue EvaluateIdent(ASTnode* node, bool insertFlag = true);
		OPValue EvaluateLocalIdent(ASTnode* node, bool insertFlag = true);
		OPValue EvaluateGlobalIdent(ASTnode* node, bool insertFlag = true);

		//lvalue member
		Value& EvaluateLvalueMemberIdent(ASTnode* node, bool insertFlag = true, Environment* env = EnvironmentHolder::getInstance()->GetCurrentEnv());
		Value& EvaluateLvalueMemberBrackets(ASTnode* node, bool insertFlag = true, Environment* env = EnvironmentHolder::getInstance()->GetCurrentEnv());
		Value& EvaluateLvalueMemberCallIdent(ASTnode* node, bool insertFlag = true, Environment* env = EnvironmentHolder::getInstance()->GetCurrentEnv());
		Value& EvaluateLvalueMemberCallBrackets(ASTnode* node, bool insertFlag = true, Environment* env = EnvironmentHolder::getInstance()->GetCurrentEnv());

		//member
		OPValue EvaluateMemberIdent(ASTnode* node, bool insertFlag = true);
		OPValue EvaluateMemberBrackets(ASTnode* node, bool insertFlag = true);
		OPValue EvaluateMemberCallIdent(ASTnode* node, bool insertFlag = true);
		OPValue EvaluateMemberCallBrackets(ASTnode* node, bool insertFlag = true);

		//call
		OPValue EvaluateMultiCall(ASTnode* node, bool insertFlag = true);
		OPValue EvaluateLvalueNormalCall(ASTnode* node, bool insertFlag = true);
		OPValue EvaluateLvalueMethodCall(ASTnode* node, bool insertFlag = true);
		OPValue EvaluateFuncdefCall(ASTnode* node, bool insertFlag = true);

		OPValue EvaluateNormCall(ASTnode* node, bool insertFlag = true);
		OPValue EvaluateMethodCall(ASTnode* node, bool insertFlag = true);

		//arglist
		OPValue EvaluateArglist(ASTnode* node, bool insertFlag = true);
		OPValue EvaluateEmptyArglist(ASTnode* node, bool insertFlag = true);

		// assignment
		OPValue EvaluateAssignExpr(ASTnode* node, bool insertFlag = true);

		//stmt
		OPValue EvaluateIfStmt(ASTnode* node, bool insertFlag = true);
		OPValue EvaluateIfElseStmt(ASTnode* node, bool insertFlag = true);
		OPValue EvaluateWhileStmt(ASTnode* node, bool insertFlag = true);
		OPValue EvaluateForStmt(ASTnode* node, bool insertFlag = true);
		OPValue EvaluateReturnStmt(ASTnode* node, bool insertFlag = true);
		OPValue EvaluateReturnValueStmt(ASTnode* node, bool insertFlag = true);
		OPValue EvaluateBreak(ASTnode* node, bool insertFlag = true);
		OPValue EvaluateContinue(ASTnode* node, bool insertFlag = true);
		OPValue EvaluateSemicolon(ASTnode* node, bool insertFlag = true);

		//elist
		OPValue EvaluateElist(ASTnode* node, bool insertFlag = true);
		OPValue EvaluateEmptyElist(ASTnode* node, bool insertFlag = true);

		//indexed
		OPValue EvaluateIndexed(ASTnode* node, bool insertFlag = true);

		//indexedElem
		OPValue EvaluateIndexedElem(ASTnode* node, bool insertFlag = true);

		//object
		OPValue EvaluateElistObjectdef(ASTnode* node, bool insertFlag = true);
		OPValue EvaluateIndexedObjectdef(ASTnode* node, bool insertFlag = true);

		//block
		OPValue EvaluateBlock(ASTnode* node, bool insertFlag = true);

		//funcdef
		OPValue EvaluateFuncdef(ASTnode* node, bool insertFlag = true);
		OPValue EvaluateAnonymousFuncdef(ASTnode* node, bool insertFlag = true);
		OPValue EvaluateFuncEnter(ASTnode* node, bool insertFlag = true);
		OPValue EvaluateFuncBody(ASTnode* node, bool insertFlag);

		// const
		OPValue EvaluateNumberConst(ASTnode* node, bool insertFlag = true);
		OPValue EvaluateStringConst(ASTnode* node, bool insertFlag = true);
		OPValue EvaluateBoolConst(ASTnode* node, bool insertFlag = true);
		OPValue EvaluateNIL(ASTnode* node, bool insertFlag = true);

		//formal
		OPValue EvaluateParam(ASTnode* node, bool insertFlag = true);
		OPValue EvaluateOptionalParam(ASTnode* node, bool insertFlag = true);

		//idlist
		OPValue EvaluateIdlist(ASTnode* node, bool insertFlag = true);
		OPValue EvaluateEmptyIdlist(ASTnode* node, bool insertFlag = true);

		//libFunctions
		OPValue EvaluatePrint(ASTnode* node, bool insertFlag = true);
		OPValue EvaluateTypeof(ASTnode* node, bool insertFlag = true);
		OPValue EvaluateObject_keys(ASTnode* node, bool insertFlag = true);
		OPValue EvaluateObject_size(ASTnode* node, bool insertFlag = true);
		OPValue EvaluateEval(ASTnode* node, bool insertFlag = true);

		// metaprogramming
		OPValue EvaluateSyntax(ASTnode* node, bool insertFlag = true);
		OPValue EvaluateEscape(ASTnode* node, bool insertFlag = true);
		OPValue EvaluateExecute(ASTnode* node, bool insertFlag = true);
		OPValue EvaluateParse(ASTnode* node, bool insertFlag = true);
		OPValue EvaluateUnparse(ASTnode* node, bool insertFlag = true);
		OPValue EvaluateMetaAST(ASTnode* node, bool insertFlag = true);
	};
}