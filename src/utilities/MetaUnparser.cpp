#include "utilities/MetaUnparser.h"

using namespace interpreter;

std::map<std::string, std::string(MetaUnparser::*)(ASTnode*)> MetaUnparser::IntializeDispatcher() {
	std::map<std::string, std::string(MetaUnparser::*)(ASTnode*)> table;

	table["program"] = &MetaUnparser::UnparseProgram;
	table["add"] = &MetaUnparser::UnparseAddExpr;
	table["sub"] = &MetaUnparser::UnparseSubExpr;
	table["mul"] = &MetaUnparser::UnparseMulExpr;
	table["div"] = &MetaUnparser::UnparseDivExpr;
	table["mod"] = &MetaUnparser::UnparseModExpr;
	table["greater"] = &MetaUnparser::UnparseGreaterExpr;
	table["greater_or_equal"] = &MetaUnparser::UnparseGreaterOrEqualExpr;
	table["less"] = &MetaUnparser::UnparseLessExpr;
	table["less_or_equal"] = &MetaUnparser::UnparseLessOrEqualExpr;
	table["equal"] = &MetaUnparser::UnparseEqualExpr;
	table["not_equal"] = &MetaUnparser::UnparseNotEqualExpr;
	table["and"] = &MetaUnparser::UnparseAndExpr;
	table["or"] = &MetaUnparser::UnparseOrExpr;
	table["parentheses"] = &MetaUnparser::UnparseParenthesis;
	table["uminus"] = &MetaUnparser::UnparseUminus;
	table["not"] = &MetaUnparser::UnparseNot;
	table["pre_increment"] = &MetaUnparser::UnparsePreIncrement;
	table["post_increment"] = &MetaUnparser::UnparsePostIncrement;
	table["pre_decrement"] = &MetaUnparser::UnparsePreDecrement;
	table["post_decrement"] = &MetaUnparser::UnparsePostDecrement;
	table["parentheses_funcdef"] = &MetaUnparser::UnparseParenthesisFuncdef;
	table["var"] = &MetaUnparser::UnparseIdent;
	table["localVar"] = &MetaUnparser::UnparseLocalIdent;
	table["globalVar"] = &MetaUnparser::UnparseGlobalIdent;
	table["member_lvalueVar"] = &MetaUnparser::UnparseMemberIdent;
	table["member_lvalueBrackets"] = &MetaUnparser::UnparseMemberBrackets;
	table["member_callVar"] = &MetaUnparser::UnparseMemberCallIdent;
	table["member_callBrackets"] = &MetaUnparser::UnparseMemberCallBrackets;
	table["multiCall"] = &MetaUnparser::UnparseMultiCall;
	table["lvalueNormCall"] = &MetaUnparser::UnparseLvalueNormalCall;
	table["lvalueMethodCall"] = &MetaUnparser::UnparseLvalueMethodCall;
	table["funcdefCall"] = &MetaUnparser::UnparseFuncdefCall;
	table["argList"] = &MetaUnparser::UnparseArglist;
	table["emptyArgList"] = &MetaUnparser::UnparseEmptyArglist;
	table["assignexpr"] = &MetaUnparser::UnparseAssignExpr;
	table["exprSemicolon"] = &MetaUnparser::UnparseExprSemicolon;
	table["ifstmt"] = &MetaUnparser::UnparseIfStmt;
	table["if_elsestmt"] = &MetaUnparser::UnparseIfElseStmt;
	table["whilestmt"] = &MetaUnparser::UnparseWhileStmt;
	table["forstmt"] = &MetaUnparser::UnparseForStmt;
	table["return"] = &MetaUnparser::UnparseReturnStmt;
	table["return_value"] = &MetaUnparser::UnparseReturnValueStmt;
	table["break"] = &MetaUnparser::UnparseBreak;
	table["continue"] = &MetaUnparser::UnparseContinue;
	table["semicolon"] = &MetaUnparser::UnparseSemicolon;
	table["elist"] = &MetaUnparser::UnparseElist;
	table["emptyElist"] = &MetaUnparser::UnparseEmptyElist;
	table["indexed"] = &MetaUnparser::UnparseIndexed;
	table["indexedElem"] = &MetaUnparser::UnparseIndexedElem;
	table["elistObjectdef"] = &MetaUnparser::UnparseElistObjectdef;
	table["indexedObjectdef"] = &MetaUnparser::UnparseIndexedObjectdef;
	table["block"] = &MetaUnparser::UnparseBlock;
	table["funcdef"] = &MetaUnparser::UnparseFuncdef;
	table["anonymousFuncdef"] = &MetaUnparser::UnparseAnonymousFuncdef;
	table["funcbody"] = &MetaUnparser::UnparseFuncBody;
	table["numConst"] = &MetaUnparser::UnparseNumberConst;
	table["stringConst"] = &MetaUnparser::UnparseStringConst;
	table["boolConst"] = &MetaUnparser::UnparseBoolConst;
	table["nil"] = &MetaUnparser::UnparseNIL;
	table["param"] = &MetaUnparser::UnparseParam;
	table["optionalParam"] = &MetaUnparser::UnparseOptionalParam;
	table["idlist"] = &MetaUnparser::UnparseIdlist;
	table["emptyIdlist"] = &MetaUnparser::UnparseEmptyIdlist;
	table["meta_syntax"] = &MetaUnparser::UnparseSyntax;
	table["meta_escape"] = &MetaUnparser::UnparseEscape;
	table["meta_execute"] = &MetaUnparser::UnparseExecute;
	table["meta_parse"] = &MetaUnparser::UnparseParse;
	table["meta_unparse"] = &MetaUnparser::UnparseUnparse;
	table["metaAST"] = &MetaUnparser::UnparseMetaAST;
	return table;
}


MetaUnparser::MetaUnparser() {
	UnparserDispatcher = IntializeDispatcher();
}

MetaUnparser* MetaUnparser::getInstance() {
	if (!unparser)
		unparser = new MetaUnparser();
	return unparser;
}

void  MetaUnparser::destroyInstance() {
	if(unparser)
		delete unparser;
}

//generic Unparse
std::string MetaUnparser::Unparse(ASTnode* node) {
	//lineNumber = node->GetLine();
	return (this->*UnparserDispatcher[node->GetValue("type")->GetStringValue()])(node);
}

// program
std::string MetaUnparser::UnparseProgram(ASTnode* node){
	std::string retString = "";
	double numOfStmt = node->GetValue("numOfStmt")->GetNumberValue();
	for (double i = 0; i < numOfStmt; i++) {
		std::string stmt = Unparse(node->GetValue(i)->GetObjectValue());
		retString += stmt + "\n";
	}
	return retString;
}

// expr	
std::string MetaUnparser::UnparseAddExpr(ASTnode* node){
	std::string left = Unparse(node->GetValue("left")->GetObjectValue());
	std::string right = Unparse(node->GetValue("right")->GetObjectValue());
	return left + " + " + right;
}

std::string MetaUnparser::UnparseSubExpr(ASTnode* node){
	std::string left = Unparse(node->GetValue("left")->GetObjectValue());
	std::string right = Unparse(node->GetValue("right")->GetObjectValue());
	return left + " - " + right;
}

std::string MetaUnparser::UnparseMulExpr(ASTnode* node){
	std::string left = Unparse(node->GetValue("left")->GetObjectValue());
	std::string right = Unparse(node->GetValue("right")->GetObjectValue());
	return left + " * " + right;
}

std::string MetaUnparser::UnparseDivExpr(ASTnode* node){
	std::string left = Unparse(node->GetValue("left")->GetObjectValue());
	std::string right = Unparse(node->GetValue("right")->GetObjectValue());
	return left + " / " + right;
}

std::string MetaUnparser::UnparseModExpr(ASTnode* node){
	std::string left = Unparse(node->GetValue("left")->GetObjectValue());
	std::string right = Unparse(node->GetValue("right")->GetObjectValue());
	return left + " % " + right;
}

std::string MetaUnparser::UnparseGreaterExpr(ASTnode* node){
	std::string left = Unparse(node->GetValue("left")->GetObjectValue());
	std::string right = Unparse(node->GetValue("right")->GetObjectValue());
	return left + " > " + right;
}

std::string MetaUnparser::UnparseGreaterOrEqualExpr(ASTnode* node){
	std::string left = Unparse(node->GetValue("left")->GetObjectValue());
	std::string right = Unparse(node->GetValue("right")->GetObjectValue());
	return left + " >= " + right;
}

std::string MetaUnparser::UnparseLessExpr(ASTnode* node){
	std::string left = Unparse(node->GetValue("left")->GetObjectValue());
	std::string right = Unparse(node->GetValue("right")->GetObjectValue());
	return left + " < " + right;
}

std::string MetaUnparser::UnparseLessOrEqualExpr(ASTnode* node){
	std::string left = Unparse(node->GetValue("left")->GetObjectValue());
	std::string right = Unparse(node->GetValue("right")->GetObjectValue());
	return left + " <= " + right;
}

std::string MetaUnparser::UnparseEqualExpr(ASTnode* node){
	std::string left = Unparse(node->GetValue("left")->GetObjectValue());
	std::string right = Unparse(node->GetValue("right")->GetObjectValue());
	return left + " == " + right;
}

std::string MetaUnparser::UnparseNotEqualExpr(ASTnode* node){
	std::string left = Unparse(node->GetValue("left")->GetObjectValue());
	std::string right = Unparse(node->GetValue("right")->GetObjectValue());
	return left + " != " + right;
}

std::string MetaUnparser::UnparseAndExpr(ASTnode* node){
	std::string left = Unparse(node->GetValue("left")->GetObjectValue());
	std::string right = Unparse(node->GetValue("right")->GetObjectValue());
	return left + " && " + right;
}

std::string MetaUnparser::UnparseOrExpr(ASTnode* node){
	std::string left = Unparse(node->GetValue("left")->GetObjectValue());
	std::string right = Unparse(node->GetValue("right")->GetObjectValue());
	return left + " || " + right;
}

// term		
std::string MetaUnparser::UnparseParenthesis(ASTnode* node){
	std::string expr = Unparse(node->GetValue("expr")->GetObjectValue());
	return "(" + expr + ")";
}

std::string MetaUnparser::UnparseUminus(ASTnode* node){
	std::string expr = Unparse(node->GetValue("expr")->GetObjectValue());
	return "-" + expr;
}

std::string MetaUnparser::UnparseNot(ASTnode* node){
	std::string expr = Unparse(node->GetValue("expr")->GetObjectValue());
	return "!" + expr;
}

std::string MetaUnparser::UnparsePreIncrement(ASTnode* node){
	std::string lvalue = Unparse(node->GetValue("lvalue")->GetObjectValue());
	return "++" + lvalue;
}

std::string MetaUnparser::UnparsePostIncrement(ASTnode* node){
	std::string lvalue = Unparse(node->GetValue("lvalue")->GetObjectValue());
	return lvalue + "++";
}

std::string MetaUnparser::UnparsePreDecrement(ASTnode* node){
	std::string lvalue = Unparse(node->GetValue("lvalue")->GetObjectValue());
	return "--" + lvalue;
}

std::string MetaUnparser::UnparsePostDecrement(ASTnode* node){
	std::string lvalue = Unparse(node->GetValue("lvalue")->GetObjectValue());
	return lvalue + "--";
}

// primary	
std::string MetaUnparser::UnparseParenthesisFuncdef(ASTnode* node){
	std::string idlist = Unparse(node->GetValue("funcEnter")->GetObjectValue()->GetValue("idlist")->GetObjectValue());
	std::string funcbody = Unparse(node->GetValue("funcEnter")->GetObjectValue()->GetValue("funcbody")->GetObjectValue());
	if (!node->HasProperty("ID")) {
		return "(function (" + idlist + ") " + funcbody + ")";
	}
	else {
		std::string id = node->GetValue("ID")->GetStringValue();
		if (id.rfind("$", 0) == 0) {
			return "(function (" + idlist + ") " + funcbody + ")";
		}
		return "(function " + id + "(" + idlist + ") " + funcbody + ")";
	}

}


//rvalue	
std::string MetaUnparser::UnparseIdent(ASTnode* node){
	return node->GetValue("ID")->GetStringValue();
}
std::string MetaUnparser::UnparseLocalIdent(ASTnode* node){
	return "local " + node->GetValue("ID")->GetStringValue();
}
std::string MetaUnparser::UnparseGlobalIdent(ASTnode* node){
	return "::" + node->GetValue("ID")->GetStringValue();
}

//member	
std::string MetaUnparser::UnparseMemberIdent(ASTnode* node){
	std::string lvalue = Unparse(node->GetValue("lvalue")->GetObjectValue());
	return lvalue + "." + node->GetValue("ID")->GetStringValue();
}

std::string MetaUnparser::UnparseMemberBrackets(ASTnode* node){
	std::string lvalue = Unparse(node->GetValue("lvalue")->GetObjectValue());
	std::string expr = Unparse(node->GetValue("expr")->GetObjectValue());
	return lvalue + "[" + expr + "]";
}

std::string MetaUnparser::UnparseMemberCallIdent(ASTnode* node){
	std::string call = Unparse(node->GetValue("call")->GetObjectValue());
	return call + "." + node->GetValue("ID")->GetStringValue();
}

std::string MetaUnparser::UnparseMemberCallBrackets(ASTnode* node){
	std::string call = Unparse(node->GetValue("call")->GetObjectValue());
	std::string expr = Unparse(node->GetValue("expr")->GetObjectValue());
	return call + "[" + expr + "]";
}
	
//call		
std::string MetaUnparser::UnparseMultiCall(ASTnode* node){
	std::string call = Unparse(node->GetValue("call")->GetObjectValue());
	std::string argList = Unparse(node->GetValue("argList")->GetObjectValue());
	return call + "(" + argList + ")";
}

std::string MetaUnparser::UnparseLvalueNormalCall(ASTnode* node){
	std::string lvalue = Unparse(node->GetValue("lvalue")->GetObjectValue());
	std::string argList = Unparse(node->GetValue("argList")->GetObjectValue());
	return lvalue + "(" + argList + ")";
}

std::string MetaUnparser::UnparseLvalueMethodCall(ASTnode* node){
	std::string lvalue = Unparse(node->GetValue("lvalue")->GetObjectValue());
	std::string id = node->GetValue("ID")->GetStringValue();
	std::string arglist = Unparse(node->GetValue("argList")->GetObjectValue());
	return lvalue + ".." + id + "(" + arglist + ")";
}

std::string MetaUnparser::UnparseFuncdefCall(ASTnode* node){
	std::string funcdef = Unparse(node->GetValue("funcdef")->GetObjectValue());
	std::string arglist = Unparse(node->GetValue("argList")->GetObjectValue());
	return "(" + funcdef + ")(" + arglist + ")";
}

//std::string MetaUnparser::UnparseNormCall(ASTnode* node){}
//std::string MetaUnparser::UnparseMethodCall(ASTnode* node){}

//arglist	
std::string MetaUnparser::UnparseArglist(ASTnode* node){
	std::string retString = "";
	int iterationNamed = 0;
	double numOfTotalArgs = node->GetValue("numOfTotalArgs")->GetNumberValue();
	double numOfPositionalArgs = node->GetValue("numOfPositionalArgs")->GetNumberValue();
	Object* PositionalArgs = node->GetValue("PositionalArgs")->GetObjectValue();
	Object* NamedArgs = node->GetValue("NamedArgs")->GetObjectValue();

	for (double i = 0; i < numOfPositionalArgs; ++i) {
		if(i==0)
			retString += Unparse(PositionalArgs->GetValue(i)->GetObjectValue());
		else
			retString += ", " + Unparse(PositionalArgs->GetValue(i)->GetObjectValue());
	}

	for (auto kv : NamedArgs->GetMap()) {
		if (iterationNamed == 0 && numOfPositionalArgs == 0.0)
			retString += kv.first.toString() + " : " + Unparse(kv.second.GetObjectValue());
		else
			retString += ", " + kv.first.toString() + " : " + Unparse(kv.second.GetObjectValue());
		iterationNamed++;
	}
	return retString;
}

std::string MetaUnparser::UnparseEmptyArglist(ASTnode* node){
	return "";
}

// assignment
std::string MetaUnparser::UnparseAssignExpr(ASTnode* node){
	std::string lvalue = Unparse(node->GetValue("lvalue")->GetObjectValue());
	std::string expr = Unparse(node->GetValue("expr")->GetObjectValue());
	return lvalue + " = " + expr;
}

//stmt		
std::string MetaUnparser::UnparseExprSemicolon(ASTnode* node) {
	std::string expr = Unparse(node->GetValue("expr")->GetObjectValue());
	return expr + ";";
}

std::string MetaUnparser::UnparseIfStmt(ASTnode* node){
	std::string condition = Unparse(node->GetValue("condition")->GetObjectValue());
	std::string stmt = Unparse(node->GetValue("stmt")->GetObjectValue());
	return "if ( " + condition + " ) \n" + stmt;
}

std::string MetaUnparser::UnparseIfElseStmt(ASTnode* node){
	std::string ifstmt = Unparse(node->GetValue("ifstmt")->GetObjectValue());
	std::string elsestmt = Unparse(node->GetValue("elsestmt")->GetObjectValue());
	return ifstmt + "\n else \n" + elsestmt;
}

std::string MetaUnparser::UnparseWhileStmt(ASTnode* node){
	std::string condition = Unparse(node->GetValue("condition")->GetObjectValue());
	std::string stmt = Unparse(node->GetValue("stmt")->GetObjectValue());
	return "while (" + condition + ") \n" + stmt;
}

std::string MetaUnparser::UnparseForStmt(ASTnode* node){
	std::string init_elist = Unparse(node->GetValue("init_elist")->GetObjectValue());
	std::string condition = Unparse(node->GetValue("condition")->GetObjectValue());
	std::string elist = Unparse(node->GetValue("elist")->GetObjectValue());
	std::string stmt = Unparse(node->GetValue("stmt")->GetObjectValue());
	return "for (" + init_elist + "; " + condition + "; " + elist + ") \n" + stmt;
}

std::string MetaUnparser::UnparseReturnStmt(ASTnode* node){
	return "return;";
}

std::string MetaUnparser::UnparseReturnValueStmt(ASTnode* node){
	std::string expr = Unparse(node->GetValue("expr")->GetObjectValue());
	return "return " + expr + ";" ;
}

std::string MetaUnparser::UnparseBreak(ASTnode* node){
	return "break;";
}

std::string MetaUnparser::UnparseContinue(ASTnode* node){
	return "continue;";
}

std::string MetaUnparser::UnparseSemicolon(ASTnode* node){
	return ";";
}
	
//elist		
std::string MetaUnparser::UnparseElist(ASTnode* node){
	std::string retString = "";
	double numOfExprs = node->GetValue("numOfExprs")->GetNumberValue();
	for (double i = 0; i < numOfExprs; i++) {
		std::string expr = Unparse(node->GetValue(i)->GetObjectValue());
		if(i==0)
			retString += expr;
		else
			retString += ", " + expr;
	}
	return retString;
}

std::string MetaUnparser::UnparseEmptyElist(ASTnode* node){
	return "";
}
	
//indexed	
std::string MetaUnparser::UnparseIndexed(ASTnode* node){
	std::string retString = "";
	double numOfElems = node->GetValue("numOfElems")->GetNumberValue();
	for (double i = 0; i < numOfElems; i++) {
		std::string indexed = Unparse(node->GetValue(i)->GetObjectValue());
		if (i == 0)
			retString += indexed;
		else
			retString += ", " + indexed;
	}
	return retString;
}
	
//indexedElem
std::string MetaUnparser::UnparseIndexedElem(ASTnode* node){
	std::string keyExpr = Unparse(node->GetValue("keyExpr")->GetObjectValue());
	std::string valueExpr = Unparse(node->GetValue("valueExpr")->GetObjectValue());
	return "{" + keyExpr + ":" + valueExpr + "}";
}
	
//object	
std::string MetaUnparser::UnparseElistObjectdef(ASTnode* node){
	std::string elist = Unparse(node->GetValue("elist")->GetObjectValue());
	return "[" + elist + "]";
}

std::string MetaUnparser::UnparseIndexedObjectdef(ASTnode* node){
	std::string indexed = Unparse(node->GetValue("indexed")->GetObjectValue());
	return "[" + indexed + "]";
}
	
//block		
std::string MetaUnparser::UnparseBlock(ASTnode* node){
	std::string retString = "{\n";
	double numOfStmt = node->GetValue("numOfStmt")->GetNumberValue();
	for (double i = 0; i < numOfStmt; i++) {
		std::string stmt = Unparse(node->GetValue(i)->GetObjectValue());
		retString += stmt + "\n";
	}
	retString += "}\n";
	return retString;
}

//funcdef	
std::string MetaUnparser::UnparseFuncdef(ASTnode* node){
	std::string id = node->GetValue("ID")->GetStringValue();
	std::string idlist = Unparse(node->GetValue("funcEnter")->GetObjectValue()->GetValue("idlist")->GetObjectValue());
	std::string funcbody = Unparse(node->GetValue("funcEnter")->GetObjectValue()->GetValue("funcbody")->GetObjectValue());
	return "function " + id + "(" + idlist + ") \n" + funcbody;
}

std::string MetaUnparser::UnparseAnonymousFuncdef(ASTnode* node){
	std::string idlist = Unparse(node->GetValue("funcEnter")->GetObjectValue()->GetValue("idlist")->GetObjectValue());
	std::string funcbody = Unparse(node->GetValue("funcEnter")->GetObjectValue()->GetValue("funcbody")->GetObjectValue());
	return "function (" + idlist + ") \n" + funcbody;
}

//std::string MetaUnparser::UnparseFuncEnter(ASTnode* node){}

std::string MetaUnparser::UnparseFuncBody(ASTnode* node){
	std::string retString = "{\n";
	double numOfStmt = node->GetValue("numOfStmt")->GetNumberValue();
	for (double i = 0; i < numOfStmt; i++) {
		std::string stmt = Unparse(node->GetValue(i)->GetObjectValue());
		retString += stmt + "\n";
	}
	retString += "}\n";
	return retString;
}

// const	
std::string MetaUnparser::UnparseNumberConst(ASTnode* node){
	return std::to_string(node->GetValue("value")->GetNumberValue());
}

std::string MetaUnparser::UnparseStringConst(ASTnode* node){
	return "\"" + node->GetValue("value")->GetStringValue() + "\"";
}

std::string MetaUnparser::UnparseBoolConst(ASTnode* node){
	return std::to_string(node->GetValue("value")->GetBoolValue());
}

std::string MetaUnparser::UnparseNIL(ASTnode* node){
	return "nil";
}

//formal	
std::string MetaUnparser::UnparseParam(ASTnode* node){
	return node->GetValue("ID")->GetStringValue();
}

std::string MetaUnparser::UnparseOptionalParam(ASTnode* node){
	std::string expr = Unparse(node->GetValue("expr")->GetObjectValue());
	return node->GetValue("ID")->GetStringValue() + " = " + expr;
}

//idlist	
std::string MetaUnparser::UnparseIdlist(ASTnode* node){
	std::string retString = "";
	double numOfParams = node->GetValue("numOfParams")->GetNumberValue();
	for (double i = 0; i < numOfParams; i++) {
		std::string formal = Unparse(node->GetValue(i)->GetObjectValue());
		if (i == 0)
			retString += formal;
		else
			retString += ", " + formal;
	}
	return retString;
}

std::string MetaUnparser::UnparseEmptyIdlist(ASTnode* node){
	return "";
}
	


// metaprogramming
std::string MetaUnparser::UnparseSyntax(ASTnode* node){
	std::string expr = Unparse(node->GetValue("expr")->GetObjectValue());
	return ".<" + expr + ">.";
}

std::string MetaUnparser::UnparseEscape(ASTnode* node){
	//std::string id = node->GetValue("ID")->GetStringValue();
	std::string meta_var = Unparse(node->GetValue("meta_var")->GetObjectValue());
	return ".~" + meta_var;
}

std::string MetaUnparser::UnparseExecute(ASTnode* node){
	std::string expr = Unparse(node->GetValue("expr")->GetObjectValue());
	return ".!" + expr;
}

std::string MetaUnparser::UnparseParse(ASTnode* node){
	std::string str = node->GetValue("stringConst")->GetStringValue();
	return ".@" + str;
}

std::string MetaUnparser::UnparseUnparse(ASTnode* node){
	std::string expr = Unparse(node->GetValue("expr")->GetObjectValue());
	return ".#" + expr;
}

std::string MetaUnparser::UnparseMetaAST(ASTnode* node) {
	std::string root = Unparse(node->GetValue("root")->GetObjectValue());
	return root;
}