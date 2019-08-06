#pragma once

#include <string>
#include <map>
#include <optional>
#include <functional>
#include "utilities/Exceptions.h"
#include "utilities/Object.h"
#include "utilities/AST.h"
#include "utilities/EvaluatorActions.h"
#include "utilities/Evaluator.h"

namespace interpreter {

	class SyntaxParser {
	public:
		static SyntaxParser* getInstance();
		static void destroyInstance();
		ASTnode* SyntaxParse(ASTnode* node);

	private:
		std::map<std::string, ASTnode*(SyntaxParser::*)(ASTnode*)> SyntaxParserDispatcher;
		std::map<std::string, ASTnode*(SyntaxParser::*)(ASTnode*)> IntializeDispatcher();
		inline static SyntaxParser* syntaxParser = nullptr;
		SyntaxParser();
		

		// expr
		ASTnode* SyntaxParseAddExpr(ASTnode* node);
		ASTnode* SyntaxParseSubExpr(ASTnode* node);
		ASTnode* SyntaxParseMulExpr(ASTnode* node);
		ASTnode* SyntaxParseDivExpr(ASTnode* node);
		ASTnode* SyntaxParseModExpr(ASTnode* node);
		ASTnode* SyntaxParseGreaterExpr(ASTnode* node);
		ASTnode* SyntaxParseGreaterOrEqualExpr(ASTnode* node);
		ASTnode* SyntaxParseLessExpr(ASTnode* node);
		ASTnode* SyntaxParseLessOrEqualExpr(ASTnode* node);
		ASTnode* SyntaxParseEqualExpr(ASTnode* node);
		ASTnode* SyntaxParseNotEqualExpr(ASTnode* node);
		ASTnode* SyntaxParseAndExpr(ASTnode* node);
		ASTnode* SyntaxParseOrExpr(ASTnode* node);


		// term
		ASTnode* SyntaxParseParenthesis(ASTnode* node);
		ASTnode* SyntaxParseUminus(ASTnode* node);
		ASTnode* SyntaxParseNot(ASTnode* node);
		ASTnode* SyntaxParsePreIncrement(ASTnode* node);
		ASTnode* SyntaxParsePostIncrement(ASTnode* node);
		ASTnode* SyntaxParsePreDecrement(ASTnode* node);
		ASTnode* SyntaxParsePostDecrement(ASTnode* node);

		// primary
		ASTnode* SyntaxParseParenthesisFuncdef(ASTnode* node);

		//rvalue
		ASTnode* SyntaxParseIdent(ASTnode* node);
		ASTnode* SyntaxParseLocalIdent(ASTnode* node);
		ASTnode* SyntaxParseGlobalIdent(ASTnode* node);

		//lvalue member
		Value& SyntaxParseLvalueMemberIdent(ASTnode* node);
		Value& SyntaxParseLvalueMemberBrackets(ASTnode* node);
		Value& SyntaxParseLvalueMemberCallIdent(ASTnode* node);
		Value& SyntaxParseLvalueMemberCallBrackets(ASTnode* node);

		//member
		ASTnode* SyntaxParseMemberIdent(ASTnode* node);
		ASTnode* SyntaxParseMemberBrackets(ASTnode* node);
		ASTnode* SyntaxParseMemberCallIdent(ASTnode* node);
		ASTnode* SyntaxParseMemberCallBrackets(ASTnode* node);

		//call
		ASTnode* SyntaxParseMultiCall(ASTnode* node);
		ASTnode* SyntaxParseLvalueNormalCall(ASTnode* node);
		ASTnode* SyntaxParseLvalueMethodCall(ASTnode* node);
		ASTnode* SyntaxParseFuncdefCall(ASTnode* node);
		
		//arglist
		ASTnode* SyntaxParseArglist(ASTnode* node);
		ASTnode* SyntaxParseEmptyArglist(ASTnode* node);
		
		// assignment
		ASTnode* SyntaxParseAssignExpr(ASTnode* node);
		
		//stmt
		ASTnode* SyntaxParseIfStmt(ASTnode* node);
		ASTnode* SyntaxParseIfElseStmt(ASTnode* node);
		ASTnode* SyntaxParseWhileStmt(ASTnode* node);
		ASTnode* SyntaxParseForStmt(ASTnode* node);
		ASTnode* SyntaxParseReturnStmt(ASTnode* node);
		ASTnode* SyntaxParseReturnValueStmt(ASTnode* node);
		ASTnode* SyntaxParseBreak(ASTnode* node);
		ASTnode* SyntaxParseContinue(ASTnode* node);
		ASTnode* SyntaxParseExprSemicolon(ASTnode* node);
		ASTnode* SyntaxParseSemicolon(ASTnode* node);
		
		//elist
		ASTnode* SyntaxParseElist(ASTnode* node);
		ASTnode* SyntaxParseEmptyElist(ASTnode* node);
		
		//indexed
		ASTnode* SyntaxParseIndexed(ASTnode* node);
		
		//indexedElem
		ASTnode* SyntaxParseIndexedElem(ASTnode* node);
		
		//object
		ASTnode* SyntaxParseElistObjectdef(ASTnode* node);
		ASTnode* SyntaxParseIndexedObjectdef(ASTnode* node);

		//block
		ASTnode* SyntaxParseBlock(ASTnode* node);

		//funcdef
		ASTnode* SyntaxParseFuncdef(ASTnode* node);
		ASTnode* SyntaxParseAnonymousFuncdef(ASTnode* node);
		ASTnode* SyntaxParseFuncEnter(ASTnode* node);
		ASTnode* SyntaxParseFuncBody(ASTnode* node);
		
		// const
		ASTnode* SyntaxParseNumberConst(ASTnode* node);
		ASTnode* SyntaxParseStringConst(ASTnode* node);
		ASTnode* SyntaxParseBoolConst(ASTnode* node);
		ASTnode* SyntaxParseNIL(ASTnode* node);

		//formal
		ASTnode* SyntaxParseParam(ASTnode* node);
		ASTnode* SyntaxParseOptionalParam(ASTnode* node);

		//idlist
		ASTnode* SyntaxParseIdlist(ASTnode* node);
		ASTnode* SyntaxParseEmptyIdlist(ASTnode* node);
	
		// metaprogramming
		ASTnode* SyntaxParseSyntax(ASTnode* node);
		ASTnode* SyntaxParseEscape(ASTnode* node);
		ASTnode* SyntaxParseExecute(ASTnode* node);
		ASTnode* SyntaxParseParse(ASTnode* node);
		ASTnode* SyntaxParseUnparse(ASTnode* node);
		ASTnode* SyntaxParseMetaAST(ASTnode* node);
	};
	
	ASTnode* SyntaxParseOuter(ASTnode* node);
}