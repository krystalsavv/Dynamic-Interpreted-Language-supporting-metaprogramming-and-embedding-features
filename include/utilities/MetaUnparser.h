#pragma once
#include <string>
#include <map>
#include <optional>
#include <functional>
#include "utilities/Exceptions.h"
#include "utilities/Object.h"
#include "utilities/AST.h"


namespace interpreter {

	class MetaUnparser {
	public:
		static MetaUnparser* getInstance();
		static void destroyInstance();
		//generic Unparse
		std::string Unparse(ASTnode* node);

	private:

		std::map<std::string, std::string(MetaUnparser::*)(ASTnode*)> UnparserDispatcher;
		std::map<std::string, std::string(MetaUnparser::*)(ASTnode*)> IntializeDispatcher();

		//Value& UnparseLvalue(ASTnode* node, Environment* env = EnvironmentHolder::getInstance()->GetCurrentEnv());
		//std::map<std::string, Value& (MetaUnparser::*)(ASTnode* Environment*)> UnparserLvalueDispatcher;
		//std::map<std::string, Value& (MetaUnparser::*)(ASTnode* Environment*)> IntializeLvalueDispatcher();

		inline static MetaUnparser* unparser = nullptr;
		MetaUnparser();

		// program
		std::string UnparseProgram(ASTnode* node);

		// expr
		std::string UnparseAddExpr(ASTnode* node);
		std::string UnparseSubExpr(ASTnode* node);
		std::string UnparseMulExpr(ASTnode* node);
		std::string UnparseDivExpr(ASTnode* node);
		std::string UnparseModExpr(ASTnode* node);
		std::string UnparseGreaterExpr(ASTnode* node);
		std::string UnparseGreaterOrEqualExpr(ASTnode* node);
		std::string UnparseLessExpr(ASTnode* node);
		std::string UnparseLessOrEqualExpr(ASTnode* node);
		std::string UnparseEqualExpr(ASTnode* node);
		std::string UnparseNotEqualExpr(ASTnode* node);
		std::string UnparseAndExpr(ASTnode* node);
		std::string UnparseOrExpr(ASTnode* node);


		// term
		std::string UnparseParenthesis(ASTnode* node);
		std::string UnparseUminus(ASTnode* node);
		std::string UnparseNot(ASTnode* node);
		std::string UnparsePreIncrement(ASTnode* node);
		std::string UnparsePostIncrement(ASTnode* node);
		std::string UnparsePreDecrement(ASTnode* node);
		std::string UnparsePostDecrement(ASTnode* node);

		// primary
		std::string UnparseParenthesisFuncdef(ASTnode* node);

		//lvalue
		//Value& UnparseLvalueIdent(ASTnode* node, Environment* env = EnvironmentHolder::getInstance()->GetCurrentEnv());
		//Value& UnparseLvalueLocalIdent(ASTnode* node, Environment* env = EnvironmentHolder::getInstance()->GetCurrentEnv());
		//Value& UnparseLvalueGlobalIdent(ASTnode* node, Environment* env = EnvironmentHolder::getInstance()->GetGlobalEnv());

		//rvalue
		std::string UnparseIdent(ASTnode* node);
		std::string UnparseLocalIdent(ASTnode* node);
		std::string UnparseGlobalIdent(ASTnode* node);

		//lvalue member
		//Value& UnparseLvalueMemberIdent(ASTnode* node, Environment* env = EnvironmentHolder::getInstance()->GetCurrentEnv());
		//Value& UnparseLvalueMemberBrackets(ASTnode* node, Environment* env = EnvironmentHolder::getInstance()->GetCurrentEnv());
		//Value& UnparseLvalueMemberCallIdent(ASTnode* node, Environment* env = EnvironmentHolder::getInstance()->GetCurrentEnv());
		//Value& UnparseLvalueMemberCallBrackets(ASTnode* node, Environment* env = EnvironmentHolder::getInstance()->GetCurrentEnv());

		//member
		std::string UnparseMemberIdent(ASTnode* node);
		std::string UnparseMemberBrackets(ASTnode* node);
		std::string UnparseMemberCallIdent(ASTnode* node);
		std::string UnparseMemberCallBrackets(ASTnode* node);

		//call
		std::string UnparseMultiCall(ASTnode* node);
		std::string UnparseLvalueNormalCall(ASTnode* node);
		std::string UnparseLvalueMethodCall(ASTnode* node);
		std::string UnparseFuncdefCall(ASTnode* node);

		std::string UnparseNormCall(ASTnode* node);
		std::string UnparseMethodCall(ASTnode* node);

		//arglist
		std::string UnparseArglist(ASTnode* node);
		std::string UnparseEmptyArglist(ASTnode* node);

		// assignment
		std::string UnparseAssignExpr(ASTnode* node);

		//stmt
		std::string UnparseIfStmt(ASTnode* node);
		std::string UnparseIfElseStmt(ASTnode* node);
		std::string UnparseWhileStmt(ASTnode* node);
		std::string UnparseForStmt(ASTnode* node);
		std::string UnparseReturnStmt(ASTnode* node);
		std::string UnparseReturnValueStmt(ASTnode* node);
		std::string UnparseBreak(ASTnode* node);
		std::string UnparseContinue(ASTnode* node);
		std::string UnparseSemicolon(ASTnode* node);

		//elist
		std::string UnparseElist(ASTnode* node);
		std::string UnparseEmptyElist(ASTnode* node);

		//indexed
		std::string UnparseIndexed(ASTnode* node);

		//indexedElem
		std::string UnparseIndexedElem(ASTnode* node);

		//object
		std::string UnparseElistObjectdef(ASTnode* node);
		std::string UnparseIndexedObjectdef(ASTnode* node);

		//block
		std::string UnparseBlock(ASTnode* node);

		//funcdef
		std::string UnparseFuncdef(ASTnode* node);
		std::string UnparseAnonymousFuncdef(ASTnode* node);
		std::string UnparseFuncEnter(ASTnode* node);
		std::string UnparseFuncBody(ASTnode* node);

		// const
		std::string UnparseNumberConst(ASTnode* node);
		std::string UnparseStringConst(ASTnode* node);
		std::string UnparseBoolConst(ASTnode* node);
		std::string UnparseNIL(ASTnode* node);

		//formal
		std::string UnparseParam(ASTnode* node);
		std::string UnparseOptionalParam(ASTnode* node);

		//idlist
		std::string UnparseIdlist(ASTnode* node);
		std::string UnparseEmptyIdlist(ASTnode* node);


		// metaprogramming
		std::string UnparseSyntax(ASTnode* node);
		std::string UnparseEscape(ASTnode* node);
		std::string UnparseExecute(ASTnode* node);
		std::string UnparseParse(ASTnode* node);
		std::string UnparseUnparse(ASTnode* node);
		std::string UnparseMetaAST(ASTnode* node);
	};

}