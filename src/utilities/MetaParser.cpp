#include "utilities/MetaParser.h"

using namespace interpreter;

std::map<std::string, ASTnode*(SyntaxParser::*)(ASTnode*)> SyntaxParser::IntializeDispatcher() {
	std::map<std::string, ASTnode*(SyntaxParser::*)(ASTnode*)> table;

	table["add"] = &SyntaxParser::SyntaxParseAddExpr;
	table["sub"] = &SyntaxParser::SyntaxParseSubExpr;
	table["mul"] = &SyntaxParser::SyntaxParseMulExpr;
	table["div"] = &SyntaxParser::SyntaxParseDivExpr;
	table["mod"] = &SyntaxParser::SyntaxParseModExpr;
	table["greater"] = &SyntaxParser::SyntaxParseGreaterExpr;
	table["greater_or_equal"] = &SyntaxParser::SyntaxParseGreaterOrEqualExpr;
	table["less"] = &SyntaxParser::SyntaxParseLessExpr;
	table["less_or_equal"] = &SyntaxParser::SyntaxParseLessOrEqualExpr;
	table["equal"] = &SyntaxParser::SyntaxParseEqualExpr;
	table["not_equal"] = &SyntaxParser::SyntaxParseNotEqualExpr;
	table["and"] = &SyntaxParser::SyntaxParseAndExpr;
	table["or"] = &SyntaxParser::SyntaxParseOrExpr;
	table["parentheses"] = &SyntaxParser::SyntaxParseParenthesis;
	table["uminus"] = &SyntaxParser::SyntaxParseUminus;
	table["not"] = &SyntaxParser::SyntaxParseNot;
	table["pre_increment"] = &SyntaxParser::SyntaxParsePreIncrement;
	table["post_increment"] = &SyntaxParser::SyntaxParsePostIncrement;
	table["pre_decrement"] = &SyntaxParser::SyntaxParsePreDecrement;
	table["post_decrement"] = &SyntaxParser::SyntaxParsePostDecrement;
	table["parentheses_funcdef"] = &SyntaxParser::SyntaxParseParenthesisFuncdef;
	table["var"] = &SyntaxParser::SyntaxParseIdent;
	table["localVar"] = &SyntaxParser::SyntaxParseLocalIdent;
	table["globalVar"] = &SyntaxParser::SyntaxParseGlobalIdent;
	table["member_lvalueVar"] = &SyntaxParser::SyntaxParseMemberIdent;
	table["member_lvalueBrackets"] = &SyntaxParser::SyntaxParseMemberBrackets;
	table["member_callVar"] = &SyntaxParser::SyntaxParseMemberCallIdent;
	table["member_callBrackets"] = &SyntaxParser::SyntaxParseMemberCallBrackets;
	table["multiCall"] = &SyntaxParser::SyntaxParseMultiCall;
	table["lvalueNormCall"] = &SyntaxParser::SyntaxParseLvalueNormalCall;
	table["lvalueMethodCall"] = &SyntaxParser::SyntaxParseLvalueMethodCall;
	table["funcdefCall"] = &SyntaxParser::SyntaxParseFuncdefCall;
	table["argList"] = &SyntaxParser::SyntaxParseArglist;
	table["emptyArgList"] = &SyntaxParser::SyntaxParseEmptyArglist;
	table["assignexpr"] = &SyntaxParser::SyntaxParseAssignExpr;
	table["ifstmt"] = &SyntaxParser::SyntaxParseIfStmt;
	table["if_elsestmt"] = &SyntaxParser::SyntaxParseIfElseStmt;
	table["whilestmt"] = &SyntaxParser::SyntaxParseWhileStmt;
	table["forstmt"] = &SyntaxParser::SyntaxParseForStmt;
	table["return"] = &SyntaxParser::SyntaxParseReturnStmt;
	table["return_value"] = &SyntaxParser::SyntaxParseReturnValueStmt;
	table["break"] = &SyntaxParser::SyntaxParseBreak;
	table["continue"] = &SyntaxParser::SyntaxParseContinue;
	table["exprSemicolon"] = &SyntaxParser::SyntaxParseExprSemicolon;
	table["semicolon"] = &SyntaxParser::SyntaxParseSemicolon;
	table["elist"] = &SyntaxParser::SyntaxParseElist;
	table["emptyElist"] = &SyntaxParser::SyntaxParseEmptyElist;
	table["indexed"] = &SyntaxParser::SyntaxParseIndexed;
	table["indexedElem"] = &SyntaxParser::SyntaxParseIndexedElem;
	table["elistObjectdef"] = &SyntaxParser::SyntaxParseElistObjectdef;
	table["indexedObjectdef"] = &SyntaxParser::SyntaxParseIndexedObjectdef;
	table["block"] = &SyntaxParser::SyntaxParseBlock;
	table["funcdef"] = &SyntaxParser::SyntaxParseFuncdef;
	table["anonymousFuncdef"] = &SyntaxParser::SyntaxParseAnonymousFuncdef;
	table["funcEnter"] = &SyntaxParser::SyntaxParseFuncEnter;
	table["funcbody"] = &SyntaxParser::SyntaxParseFuncBody;
	table["numConst"] = &SyntaxParser::SyntaxParseNumberConst;
	table["stringConst"] = &SyntaxParser::SyntaxParseStringConst;
	table["boolConst"] = &SyntaxParser::SyntaxParseBoolConst;
	table["nil"] = &SyntaxParser::SyntaxParseNIL;
	table["param"] = &SyntaxParser::SyntaxParseParam;
	table["optionalParam"] = &SyntaxParser::SyntaxParseOptionalParam;
	table["idlist"] = &SyntaxParser::SyntaxParseIdlist;
	table["emptyIdlist"] = &SyntaxParser::SyntaxParseEmptyIdlist;
	table["meta_syntax"] = &SyntaxParser::SyntaxParseSyntax;
	table["meta_escape"] = &SyntaxParser::SyntaxParseEscape;
	table["meta_execute"] = &SyntaxParser::SyntaxParseExecute;
	table["meta_parse"] = &SyntaxParser::SyntaxParseParse;
	table["meta_unparse"] = &SyntaxParser::SyntaxParseUnparse;
	table["metaAST"] = &SyntaxParser::SyntaxParseMetaAST;

	return table;
}



SyntaxParser::SyntaxParser() {
	SyntaxParserDispatcher = IntializeDispatcher();
}

SyntaxParser* SyntaxParser::getInstance() {
	if (!syntaxParser)
		syntaxParser = new SyntaxParser();
	return syntaxParser;
}

void SyntaxParser::destroyInstance() {
	if(syntaxParser)
		delete syntaxParser;
}

ASTnode* SyntaxParser::SyntaxParse(ASTnode* node) {
	return (this->*SyntaxParserDispatcher[node->GetValue("type")->GetStringValue()])(node);
}

// expr
ASTnode* SyntaxParser::SyntaxParseAddExpr(ASTnode* node) {
	ASTnode* left = SyntaxParse(node->GetValue("left")->GetObjectValue());
	ASTnode* right = SyntaxParse(node->GetValue("right")->GetObjectValue());
	ASTnode* metaNode = new ASTnode("type", "add");
	metaNode->Set("left", left);
	metaNode->Set("right", right);
	metaNode->SetLine(node->GetLine());
	return metaNode;
}

ASTnode* SyntaxParser::SyntaxParseSubExpr(ASTnode* node) {
	ASTnode* left = SyntaxParse(node->GetValue("left")->GetObjectValue());
	ASTnode* right = SyntaxParse(node->GetValue("right")->GetObjectValue());
	ASTnode* metaNode = new ASTnode("type", "sub");
	metaNode->Set("left", left);
	metaNode->Set("right", right);
	metaNode->SetLine(node->GetLine());
	return metaNode;
}

ASTnode* SyntaxParser::SyntaxParseMulExpr(ASTnode* node) {
	ASTnode* left = SyntaxParse(node->GetValue("left")->GetObjectValue());
	ASTnode* right = SyntaxParse(node->GetValue("right")->GetObjectValue());
	ASTnode* metaNode = new ASTnode("type", "mul");
	metaNode->Set("left", left);
	metaNode->Set("right", right);
	metaNode->SetLine(node->GetLine());
	return metaNode;
}

ASTnode* SyntaxParser::SyntaxParseDivExpr(ASTnode* node) {
	ASTnode* left = SyntaxParse(node->GetValue("left")->GetObjectValue());
	ASTnode* right = SyntaxParse(node->GetValue("right")->GetObjectValue());
	ASTnode* metaNode = new ASTnode("type", "div");
	metaNode->Set("left", left);
	metaNode->Set("right", right);
	metaNode->SetLine(node->GetLine());
	return metaNode;
}

ASTnode* SyntaxParser::SyntaxParseModExpr(ASTnode* node) {
	ASTnode* left = SyntaxParse(node->GetValue("left")->GetObjectValue());
	ASTnode* right = SyntaxParse(node->GetValue("right")->GetObjectValue());
	ASTnode* metaNode = new ASTnode("type", "mod");
	metaNode->Set("left", left);
	metaNode->Set("right", right);
	metaNode->SetLine(node->GetLine());
	return metaNode;
}

	ASTnode* SyntaxParser::SyntaxParseGreaterExpr(ASTnode * node) {
		ASTnode* left = SyntaxParse(node->GetValue("left")->GetObjectValue());
		ASTnode* right = SyntaxParse(node->GetValue("right")->GetObjectValue());
		ASTnode* metaNode = new ASTnode("type", "greater");
		metaNode->Set("left", left);
		metaNode->Set("right", right);
		metaNode->SetLine(node->GetLine());
		return metaNode;
	}

ASTnode* SyntaxParser::SyntaxParseGreaterOrEqualExpr(ASTnode* node) {
	ASTnode* left = SyntaxParse(node->GetValue("left")->GetObjectValue());
	ASTnode* right = SyntaxParse(node->GetValue("right")->GetObjectValue());
	ASTnode* metaNode = new ASTnode("type", "greater_or_equal");
	metaNode->Set("left", left);
	metaNode->Set("right", right);
	metaNode->SetLine(node->GetLine());
	return metaNode;
}

ASTnode* SyntaxParser::SyntaxParseLessExpr(ASTnode* node) {
	ASTnode* left = SyntaxParse(node->GetValue("left")->GetObjectValue());
	ASTnode* right = SyntaxParse(node->GetValue("right")->GetObjectValue());
	ASTnode* metaNode = new ASTnode("type", "less");
	metaNode->Set("left", left);
	metaNode->Set("right", right);
	metaNode->SetLine(node->GetLine());
	return metaNode;
}

ASTnode* SyntaxParser::SyntaxParseLessOrEqualExpr(ASTnode* node) {
	ASTnode* left = SyntaxParse(node->GetValue("left")->GetObjectValue());
	ASTnode* right = SyntaxParse(node->GetValue("right")->GetObjectValue());
	ASTnode* metaNode = new ASTnode("type", "less_or_equal");
	metaNode->Set("left", left);
	metaNode->Set("right", right);
	metaNode->SetLine(node->GetLine());
	return metaNode;
}

ASTnode* SyntaxParser::SyntaxParseEqualExpr(ASTnode* node) {
	ASTnode* left = SyntaxParse(node->GetValue("left")->GetObjectValue());
	ASTnode* right = SyntaxParse(node->GetValue("right")->GetObjectValue());
	ASTnode* metaNode = new ASTnode("type", "equal");
	metaNode->Set("left", left);
	metaNode->Set("right", right);
	metaNode->SetLine(node->GetLine());
	return metaNode;
}

ASTnode* SyntaxParser::SyntaxParseNotEqualExpr(ASTnode* node) {
	ASTnode* left = SyntaxParse(node->GetValue("left")->GetObjectValue());
	ASTnode* right = SyntaxParse(node->GetValue("right")->GetObjectValue());
	ASTnode* metaNode = new ASTnode("type", "not_equal");
	metaNode->Set("left", left);
	metaNode->Set("right", right);
	metaNode->SetLine(node->GetLine());
	return metaNode;
}

ASTnode* SyntaxParser::SyntaxParseAndExpr(ASTnode* node) {
	ASTnode* left = SyntaxParse(node->GetValue("left")->GetObjectValue());
	ASTnode* right = SyntaxParse(node->GetValue("right")->GetObjectValue());
	ASTnode* metaNode = new ASTnode("type", "and");
	metaNode->Set("left", left);
	metaNode->Set("right", right);
	metaNode->SetLine(node->GetLine());
	return metaNode;
}

ASTnode* SyntaxParser::SyntaxParseOrExpr(ASTnode* node) {
	ASTnode* left = SyntaxParse(node->GetValue("left")->GetObjectValue());
	ASTnode* right = SyntaxParse(node->GetValue("right")->GetObjectValue());
	ASTnode* metaNode = new ASTnode("type", "or");
	metaNode->Set("left", left);
	metaNode->Set("right", right);
	metaNode->SetLine(node->GetLine());
	return metaNode;
}


// term
ASTnode* SyntaxParser::SyntaxParseParenthesis(ASTnode* node) {
	ASTnode* expr =  SyntaxParse(node->GetValue("expr")->GetObjectValue());
	ASTnode* metaNode = new ASTnode("type", "parentheses");
	metaNode->Set("expr", expr);
	metaNode->SetLine(node->GetLine());
	return metaNode;
}

ASTnode* SyntaxParser::SyntaxParseUminus(ASTnode* node) {
	ASTnode* expr = SyntaxParse(node->GetValue("expr")->GetObjectValue());
	ASTnode* metaNode = new ASTnode("type", "uminus");
	metaNode->Set("expr", expr);
	metaNode->SetLine(node->GetLine());
	return metaNode;
}

ASTnode* SyntaxParser::SyntaxParseNot(ASTnode* node) {
	ASTnode* expr = SyntaxParse(node->GetValue("expr")->GetObjectValue());
	ASTnode* metaNode = new ASTnode("type", "not");
	metaNode->Set("expr", expr);
	metaNode->SetLine(node->GetLine());
	return metaNode;
}


ASTnode* SyntaxParser::SyntaxParsePreIncrement(ASTnode* node) {
	ASTnode* lvalue = SyntaxParse(node->GetValue("lvalue")->GetObjectValue());
	ASTnode* metaNode = new ASTnode("type", "pre_increment");
	metaNode->Set("lvalue", lvalue);
	metaNode->SetLine(node->GetLine());
	return metaNode;
}

ASTnode* SyntaxParser::SyntaxParsePostIncrement(ASTnode* node) {
	ASTnode* lvalue = SyntaxParse(node->GetValue("lvalue")->GetObjectValue());
	ASTnode* metaNode = new ASTnode("type", "post_increment");
	metaNode->Set("lvalue", lvalue);
	metaNode->SetLine(node->GetLine());
	return metaNode;
}

ASTnode* SyntaxParser::SyntaxParsePreDecrement(ASTnode* node) {
	ASTnode* lvalue = SyntaxParse(node->GetValue("lvalue")->GetObjectValue());
	ASTnode* metaNode = new ASTnode("type", "pre_decrement");
	metaNode->Set("lvalue", lvalue);
	metaNode->SetLine(node->GetLine());
	return metaNode;
}

ASTnode* SyntaxParser::SyntaxParsePostDecrement(ASTnode* node) {
	ASTnode* lvalue = SyntaxParse(node->GetValue("lvalue")->GetObjectValue());
	ASTnode* metaNode = new ASTnode("type", "post_decrement");
	metaNode->Set("lvalue", lvalue);
	metaNode->SetLine(node->GetLine());
	return metaNode;
}


// primary
ASTnode* SyntaxParser::SyntaxParseParenthesisFuncdef(ASTnode* node) {
	ASTnode* funcEnter = SyntaxParse(node->GetValue("funcEnter")->GetObjectValue());
	ASTnode* metaNode = new ASTnode("type", "parentheses_funcdef");
	if(node->HasProperty("ID"))
		metaNode->Set("ID", *node->GetValue("ID"));
	metaNode->Set("funcEnter", funcEnter);
	metaNode->SetLine(node->GetLine());
	return metaNode;
}

ASTnode* SyntaxParser::SyntaxParseIdent(ASTnode* node) {
	ASTnode* metaNode = new ASTnode("type", "var");
	metaNode->Set("ID", *node->GetValue("ID"));
	metaNode->SetLine(node->GetLine());
	return metaNode;
}

ASTnode* SyntaxParser::SyntaxParseLocalIdent(ASTnode* node) {
	ASTnode* metaNode = new ASTnode("type", "localVar");
	metaNode->Set("ID", *node->GetValue("ID"));
	metaNode->SetLine(node->GetLine());
	return metaNode;
}

ASTnode* SyntaxParser::SyntaxParseGlobalIdent(ASTnode* node) {
	ASTnode* metaNode = new ASTnode("type", "globalVar");
	metaNode->Set("ID", *node->GetValue("ID"));
	metaNode->SetLine(node->GetLine());
	return metaNode;
}

//member
ASTnode* SyntaxParser::SyntaxParseMemberIdent(ASTnode* node) {
	ASTnode* lvalue = SyntaxParse(node->GetValue("lvalue")->GetObjectValue());
	ASTnode* metaNode = new ASTnode("type", "member_lvalueVar");
	metaNode->Set("lvalue", lvalue);
	metaNode->Set("ID", *node->GetValue("ID"));
	metaNode->SetLine(node->GetLine());
	return metaNode;
}

ASTnode* SyntaxParser::SyntaxParseMemberBrackets(ASTnode* node) {
	ASTnode* lvalue = SyntaxParse(node->GetValue("lvalue")->GetObjectValue());
	ASTnode* expr = SyntaxParse(node->GetValue("expr")->GetObjectValue());
	ASTnode* metaNode = new ASTnode("type", "member_lvalueBrackets");
	metaNode->Set("lvalue", lvalue);
	metaNode->Set("expr", expr);
	metaNode->SetLine(node->GetLine());
	return metaNode;
}

ASTnode* SyntaxParser::SyntaxParseMemberCallIdent(ASTnode* node) {
	ASTnode* call = SyntaxParse(node->GetValue("call")->GetObjectValue());
	ASTnode* metaNode = new ASTnode("type", "member_callVar");
	metaNode->Set("call", call);
	metaNode->Set("ID", *node->GetValue("ID"));
	metaNode->SetLine(node->GetLine());
	return metaNode;
}

ASTnode* SyntaxParser::SyntaxParseMemberCallBrackets(ASTnode* node) {
	ASTnode* call = SyntaxParse(node->GetValue("call")->GetObjectValue());
	ASTnode* expr = SyntaxParse(node->GetValue("expr")->GetObjectValue());
	ASTnode* metaNode = new ASTnode("type", "member_callBrackets");
	metaNode->Set("call", call);
	metaNode->Set("expr", expr);
	metaNode->SetLine(node->GetLine());
	return metaNode;
}


ASTnode* SyntaxParser::SyntaxParseMultiCall(ASTnode* node) {
	ASTnode* call = SyntaxParse(node->GetValue("call")->GetObjectValue());
	ASTnode* argList = SyntaxParse(node->GetValue("argList")->GetObjectValue());
	ASTnode* metaNode = new ASTnode("type", "multiCall");
	metaNode->Set("call", call);
	metaNode->Set("argList", argList);
	metaNode->SetLine(node->GetLine());
	return metaNode;
}


ASTnode* SyntaxParser::SyntaxParseLvalueNormalCall(ASTnode* node) {
	ASTnode* lvalue = SyntaxParse(node->GetValue("lvalue")->GetObjectValue());
	ASTnode* argList = SyntaxParse(node->GetValue("argList")->GetObjectValue());
	ASTnode* metaNode = new ASTnode("type", "lvalueNormCall");
	metaNode->Set("lvalue", lvalue);
	metaNode->Set("argList", argList);
	metaNode->SetLine(node->GetLine());
	return metaNode;
}

ASTnode* SyntaxParser::SyntaxParseLvalueMethodCall(ASTnode* node) {
	ASTnode* lvalue = SyntaxParse(node->GetValue("lvalue")->GetObjectValue());
	ASTnode* argList = SyntaxParse(node->GetValue("argList")->GetObjectValue());
	ASTnode* metaNode = new ASTnode("type", "lvalueMethodCall");
	metaNode->Set("lvalue", lvalue);
	metaNode->Set("argList", argList);
	metaNode->Set("ID", *node->GetValue("ID"));
	metaNode->SetLine(node->GetLine());
	return metaNode;
}


ASTnode* SyntaxParser::SyntaxParseFuncdefCall(ASTnode* node) {
	ASTnode* funcdef = SyntaxParse(node->GetValue("funcdef")->GetObjectValue());
	ASTnode* argList = SyntaxParse(node->GetValue("argList")->GetObjectValue());
	ASTnode* metaNode = new ASTnode("type", "funcdefCall");
	metaNode->Set("funcdef", funcdef);
	metaNode->Set("argList", argList);
	metaNode->SetLine(node->GetLine());
	return metaNode;
}

//arglist
ASTnode* SyntaxParser::SyntaxParseArglist(ASTnode* node) {
	double numOfTotalArgs = node->GetValue("numOfTotalArgs")->GetNumberValue();
	double numOfPositionalArgs = node->GetValue("numOfPositionalArgs")->GetNumberValue();
	Object* PositionalArgs = node->GetValue("PositionalArgs")->GetObjectValue();
	Object* NamedArgs = node->GetValue("NamedArgs")->GetObjectValue();
	Object* myPositionalArgs = new Object();
	Object* myNamedArgs = new Object();
	for (double i = 0; i < numOfPositionalArgs; ++i) {
		myPositionalArgs->Set(i, SyntaxParse(PositionalArgs->GetValue(i)->GetObjectValue()));
	}
	for (auto kv : NamedArgs->GetMap()) {
		myNamedArgs->Set(kv.first, SyntaxParse(kv.second.GetObjectValue()));
	}
	ASTnode* metaNode = new ASTnode("type", "argList");
	metaNode->Set("numOfTotalArgs", numOfTotalArgs);
	metaNode->Set("numOfPositionalArgs", numOfPositionalArgs);
	metaNode->Set("PositionalArgs", myPositionalArgs);
	metaNode->Set("NamedArgs", myNamedArgs);
	metaNode->SetLine(node->GetLine());
	return metaNode;
}

ASTnode* SyntaxParser::SyntaxParseEmptyArglist(ASTnode* node) {
	ASTnode* metaNode = new ASTnode("type", "emptyArgList");
	metaNode->SetLine(node->GetLine());
	return metaNode;
}

ASTnode* interpreter::SyntaxParser::SyntaxParseAssignExpr(ASTnode* node)
{
	ASTnode* expr = SyntaxParse(node->GetValue("expr")->GetObjectValue());
	ASTnode* lvalue = SyntaxParse(node->GetValue("lvalue")->GetObjectValue());
	ASTnode* metaNode = new ASTnode("type", "assignexpr");
	metaNode->Set("lvalue", lvalue);
	metaNode->Set("expr", expr);
	metaNode->SetLine(node->GetLine());
	return metaNode;
}



// stmt
ASTnode* SyntaxParser::SyntaxParseIfStmt(ASTnode* node) {
	ASTnode* metaNode = new ASTnode("type", "ifstmt");
	metaNode->Set("condition", SyntaxParse(node->GetValue("condition")->GetObjectValue()));
	metaNode->Set("stmt", SyntaxParse(node->GetValue("stmt")->GetObjectValue()));
	metaNode->SetLine(node->GetLine());
	return metaNode;
}

ASTnode* SyntaxParser::SyntaxParseIfElseStmt(ASTnode* node) {
	ASTnode* metaNode = new ASTnode("type", "if_elsestmt");
	metaNode->Set("condition", SyntaxParse(node->GetValue("condition")->GetObjectValue()));
	metaNode->Set("stmt", SyntaxParse(node->GetValue("stmt")->GetObjectValue()));
	metaNode->SetLine(node->GetLine());
	return metaNode;
}

ASTnode* SyntaxParser::SyntaxParseWhileStmt(ASTnode* node) {
	ASTnode* metaNode = new ASTnode("type", "whilestmt");
	metaNode->Set("condition", SyntaxParse(node->GetValue("condition")->GetObjectValue()));
	metaNode->Set("stmt", SyntaxParse(node->GetValue("stmt")->GetObjectValue()));
	metaNode->SetLine(node->GetLine());
	return metaNode;
}

ASTnode* SyntaxParser::SyntaxParseForStmt(ASTnode* node) {
	ASTnode* metaNode = new ASTnode("type", "forstmt");
	metaNode->Set("init_elist", SyntaxParse(node->GetValue("init_elist")->GetObjectValue()));
	metaNode->Set("condition", SyntaxParse(node->GetValue("condition")->GetObjectValue()));
	metaNode->Set("elist", SyntaxParse(node->GetValue("elist")->GetObjectValue()));
	metaNode->Set("stmt", SyntaxParse(node->GetValue("stmt")->GetObjectValue()));
	metaNode->SetLine(node->GetLine());
	return metaNode;
}

ASTnode* SyntaxParser::SyntaxParseReturnStmt(ASTnode* node) {
	ASTnode* metaNode = new ASTnode("type", "return");
	metaNode->SetLine(node->GetLine());
	return metaNode;
}

ASTnode* SyntaxParser::SyntaxParseReturnValueStmt(ASTnode* node) {
	ASTnode* metaNode = new ASTnode("type", "return_value");
	metaNode->Set("expr", SyntaxParse(node->GetValue("expr")->GetObjectValue()));
	metaNode->SetLine(node->GetLine());
	return metaNode;
}

ASTnode* SyntaxParser::SyntaxParseBreak(ASTnode* node)
{
	ASTnode* metaNode = new ASTnode("type", "break");
	metaNode->SetLine(node->GetLine());
	return metaNode;
}

ASTnode* SyntaxParser::SyntaxParseContinue(ASTnode* node)
{
	ASTnode* metaNode = new ASTnode("type", "continue");
	metaNode->SetLine(node->GetLine());
	return metaNode;
}

ASTnode* SyntaxParser::SyntaxParseExprSemicolon(ASTnode* node) {
	ASTnode* metaNode = new ASTnode("type", "exprSemicolon");
	metaNode->Set("expr", SyntaxParse(node->GetValue("expr")->GetObjectValue()));
	metaNode->SetLine(node->GetLine());
	return metaNode;
}


ASTnode* SyntaxParser::SyntaxParseSemicolon(ASTnode* node) { 
	ASTnode* metaNode = new ASTnode("type", "semicolon");
	metaNode->SetLine(node->GetLine());
	return metaNode;
}

//elist
ASTnode* SyntaxParser::SyntaxParseElist(ASTnode* node) {
	ASTnode* metaNode = new ASTnode("type", "elist");
	double numOfExprs = node->GetValue("numOfExprs")->GetNumberValue();
	metaNode->Set("numOfExprs", *node->GetValue("numOfExprs"));
	for (double i = 0; i < numOfExprs; ++i) {
		metaNode->Set(i, SyntaxParse(node->GetValue(i)->GetObjectValue()));
	}
	metaNode->SetLine(node->GetLine());
	return metaNode;
}

ASTnode* SyntaxParser::SyntaxParseEmptyElist(ASTnode* node) {
	ASTnode* metaNode = new ASTnode("type", "emptyElist");
	metaNode->SetLine(node->GetLine());
	return metaNode;
}

//indexed
ASTnode* SyntaxParser::SyntaxParseIndexed(ASTnode* node) {
	double numOfElems = node->GetValue("numOfElems")->GetNumberValue();
	ASTnode* metaNode = new ASTnode("type", "indexed");
	metaNode->Set("numOfElems", numOfElems);
	metaNode->Set("expr", SyntaxParse(node->GetValue("expr")->GetObjectValue()));
	metaNode->SetLine(node->GetLine());
	for (double i = 0; i < numOfElems; i++) 
		metaNode->Set(i, SyntaxParse(node->GetValue(i)->GetObjectValue()));
	return metaNode;
}

//indexedElem
ASTnode* SyntaxParser::SyntaxParseIndexedElem(ASTnode* node) {
	ASTnode* metaNode = new ASTnode("type", "indexedElem");
	metaNode->Set("keyExpr", SyntaxParse(node->GetValue("keyExpr")->GetObjectValue()));
	metaNode->Set("valueExpr", SyntaxParse(node->GetValue("valueExpr")->GetObjectValue()));
	metaNode->SetLine(node->GetLine());
	return metaNode;
}

//object
ASTnode* SyntaxParser::SyntaxParseElistObjectdef(ASTnode* node) {
	ASTnode* metaNode = new ASTnode("type", "elistObjectdef");
	metaNode->Set("elist", SyntaxParse(node->GetValue("elist")->GetObjectValue()));
	metaNode->SetLine(node->GetLine());
	return metaNode;
}

ASTnode* SyntaxParser::SyntaxParseIndexedObjectdef(ASTnode* node) {
	ASTnode* metaNode = new ASTnode("type", "indexedObjectdef");
	metaNode->Set("indexed", SyntaxParse(node->GetValue("indexed")->GetObjectValue()));
	metaNode->SetLine(node->GetLine());
	return metaNode;
}


//block
ASTnode* SyntaxParser::SyntaxParseBlock(ASTnode* node) {
	ASTnode* metaNode = new ASTnode("type", "block");
	double numOfStmt = node->GetValue("numOfStmt")->GetNumberValue();
	metaNode->Set("numOfStmt", numOfStmt);
	metaNode->SetLine(node->GetLine());
	for (double i = 0; i < numOfStmt; i++) {
		metaNode->Set(i, SyntaxParse(node->GetValue(i)->GetObjectValue()));
	}
	return metaNode;
}

ASTnode* interpreter::SyntaxParser::SyntaxParseFuncdef(ASTnode* node) {
	ASTnode* metaNode = new ASTnode("type", "funcdef");
	metaNode->Set("ID", *node->GetValue("ID"));
	metaNode->Set("funcEnter", SyntaxParse(node->GetValue("funcEnter")->GetObjectValue()));
	metaNode->SetLine(node->GetLine());
	return metaNode;
}

ASTnode* interpreter::SyntaxParser::SyntaxParseAnonymousFuncdef(ASTnode* node) {
	ASTnode* metaNode = new ASTnode("type", "anonymousFuncdef");
	metaNode->Set("funcEnter", SyntaxParse(node->GetValue("funcEnter")->GetObjectValue()));
	metaNode->SetLine(node->GetLine());
	return metaNode;
}


ASTnode* SyntaxParser::SyntaxParseFuncEnter(ASTnode* node) {
	ASTnode* metaNode = new ASTnode("type", "funcEnter");
	metaNode->Set("idlist", SyntaxParse(node->GetValue("idlist")->GetObjectValue()));
	metaNode->Set("funcbody", SyntaxParse(node->GetValue("funcbody")->GetObjectValue()));
	metaNode->SetLine(node->GetLine());
	return metaNode;
}

ASTnode* SyntaxParser::SyntaxParseFuncBody(ASTnode* node) {
	ASTnode* metaNode = new ASTnode("type", "funcbody");
	double numOfStmt = node->GetValue("numOfStmt")->GetNumberValue();
	metaNode->Set("numOfStmt", numOfStmt);
	metaNode->SetLine(node->GetLine());
	for (double i = 0; i < numOfStmt; i++) {
		metaNode->Set(i, SyntaxParse(node->GetValue(i)->GetObjectValue()));
	}
	return metaNode;
}

// const
ASTnode* SyntaxParser::SyntaxParseNumberConst(ASTnode* node) {
	ASTnode* metaNode = new ASTnode("type", "numConst");
	metaNode->Set("value", *node->GetValue("value"));
	metaNode->SetLine(node->GetLine());
	return metaNode;
}

ASTnode* SyntaxParser::SyntaxParseStringConst(ASTnode* node) {
	ASTnode* metaNode = new ASTnode("type", "stringConst");
	metaNode->Set("value", *node->GetValue("value"));
	metaNode->SetLine(node->GetLine());
	return metaNode;
}

ASTnode* SyntaxParser::SyntaxParseBoolConst(ASTnode* node) {
	ASTnode* metaNode = new ASTnode("type", "boolConst");
	metaNode->Set("value", *node->GetValue("value"));
	metaNode->SetLine(node->GetLine());
	return metaNode;
}

ASTnode* SyntaxParser::SyntaxParseNIL(ASTnode* node) {
	ASTnode* metaNode = new ASTnode("type", "nil");
	metaNode->SetLine(node->GetLine());
	return metaNode;
}


//formal
ASTnode* SyntaxParser::SyntaxParseParam(ASTnode* node) {
	ASTnode* metaNode = new ASTnode("type", "param");
	metaNode->Set("ID", *node->GetValue("ID"));
	metaNode->SetLine(node->GetLine());
	return metaNode;
}

ASTnode* SyntaxParser::SyntaxParseOptionalParam(ASTnode* node) {
	ASTnode* metaNode = new ASTnode("type", "optionalParam");
	metaNode->Set("ID", *node->GetValue("ID"));
	metaNode->Set("expr", SyntaxParse(node->GetValue("expr")->GetObjectValue()));
	metaNode->SetLine(node->GetLine());
	return metaNode;
}

//idlist
ASTnode* SyntaxParser::SyntaxParseIdlist(ASTnode* node) {
	double numOfParams = node->GetValue("numOfParams")->GetNumberValue();
	ASTnode* metaNode = new ASTnode("type", "idlist");
	metaNode->Set("numOfParams", numOfParams);
	metaNode->SetLine(node->GetLine());
	for (double i = 0; i < numOfParams; ++i) {
		metaNode->Set(i, SyntaxParse(node->GetValue(i)->GetObjectValue()));
	}
	return metaNode;
}

ASTnode* SyntaxParser::SyntaxParseEmptyIdlist(ASTnode* node) {
	ASTnode* metaNode = new ASTnode("type", "emptyIdlist");
	metaNode->Set("numOfParams", *node->GetValue("numOfParams"));
	metaNode->SetLine(node->GetLine());
	return metaNode;
}

// metaprogramming
ASTnode* SyntaxParser::SyntaxParseSyntax(ASTnode* node) {
	ASTnode* metaNode = new ASTnode("type", "meta_syntax");
	metaNode->Set("expr", SyntaxParse(node->GetValue("expr")->GetObjectValue()));
	metaNode->SetLine(node->GetLine());
	return metaNode;
}

ASTnode* SyntaxParser::SyntaxParseEscape(ASTnode* node) {
	ASTnode* ast = Evaluator::getInstance()->Evaluate(node)->GetObjectValue();
	return ast->GetValue("root")->GetObjectValue();
}

ASTnode* SyntaxParser::SyntaxParseExecute(ASTnode* node) {
	ASTnode* metaNode = new ASTnode("type", "meta_execute");
	metaNode->Set("expr", *node->GetValue("expr"));
	metaNode->SetLine(node->GetLine());
	return metaNode;
}

ASTnode* SyntaxParser::SyntaxParseParse(ASTnode* node) {
	ASTnode* metaNode = new ASTnode("type", "meta_parse");
	metaNode->Set("stringConst", *node->GetValue("stringConst"));
	metaNode->SetLine(node->GetLine());
	return metaNode;
}

ASTnode* SyntaxParser::SyntaxParseUnparse(ASTnode* node) {
	ASTnode* metaNode = new ASTnode("type", "meta_unparse");
	metaNode->Set("expr", SyntaxParse(node->GetValue("expr")->GetObjectValue()));
	metaNode->SetLine(node->GetLine());
	return metaNode;
}

ASTnode* SyntaxParser::SyntaxParseMetaAST(ASTnode* node) {
	return SyntaxParse(node->GetValue("root")->GetObjectValue());
}


ASTnode* interpreter::SyntaxParseOuter(ASTnode* node) {
	return SyntaxParser::getInstance()->SyntaxParse(node);
}