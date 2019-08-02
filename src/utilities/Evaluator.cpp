#include "utilities/Evaluator.h"
#include "parser.hpp"
#include "scanner.h"

#define nil (Object *)nullptr

using namespace interpreter;


std::map<std::string,OPValue(Evaluator::*)(ASTnode*,bool)> Evaluator::IntializeDispatcher() {
	std::map<std::string,OPValue(Evaluator::*)(ASTnode*, bool insertFlag)> table;

	table["program"] = &Evaluator::EvaluateProgram;
	table["add"] = &Evaluator::EvaluateAddExpr;
	table["sub"] = &Evaluator::EvaluateSubExpr;
	table["mul"] = &Evaluator::EvaluateMulExpr;
	table["div"] = &Evaluator::EvaluateDivExpr;
	table["mod"] = &Evaluator::EvaluateModExpr;
	table["greater"] = &Evaluator::EvaluateGreaterExpr;
	table["greater_or_equal"] = &Evaluator::EvaluateGreaterOrEqualExpr;
	table["less"] = &Evaluator::EvaluateLessExpr;
	table["less_or_equal"] = &Evaluator::EvaluateLessOrEqualExpr;
	table["equal"] = &Evaluator::EvaluateEqualExpr;
	table["not_equal"] = &Evaluator::EvaluateNotEqualExpr;
	table["and"] = &Evaluator::EvaluateAndExpr;
	table["or"] = &Evaluator::EvaluateOrExpr;
	table["parentheses"] = &Evaluator::EvaluateParenthesis; 
	table["uminus"] = &Evaluator::EvaluateUminus;
	table["not"] = &Evaluator::EvaluateNot;
	table["pre_increment"] = &Evaluator::EvaluatePreIncrement;
	table["post_increment"] = &Evaluator::EvaluatePostIncrement;
	table["pre_decrement"] = &Evaluator::EvaluatePreDecrement;
	table["post_decrement"] = &Evaluator::EvaluatePostDecrement;
	table["parentheses_funcdef"] = &Evaluator::EvaluateParenthesisFuncdef;
	table["var"] = &Evaluator::EvaluateIdent;
	table["localVar"] = &Evaluator::EvaluateLocalIdent;
	table["globalVar"] = &Evaluator::EvaluateGlobalIdent;
	table["member_lvalueVar"] = &Evaluator::EvaluateMemberIdent;
	table["member_lvalueBrackets"] = &Evaluator::EvaluateMemberBrackets;
	table["member_callVar"] = &Evaluator::EvaluateMemberCallIdent;
	table["member_callBrackets"] = &Evaluator::EvaluateMemberCallBrackets;
	table["multiCall"] = &Evaluator::EvaluateMultiCall;
	table["lvalueNormCall"] = &Evaluator::EvaluateLvalueNormalCall;
	table["lvalueMethodCall"] = &Evaluator::EvaluateLvalueMethodCall;
	table["funcdefCall"] = &Evaluator::EvaluateFuncdefCall;
	table["argList"] = &Evaluator::EvaluateArglist;
	table["emptyArgList"] = &Evaluator::EvaluateEmptyArglist;
	table["assignexpr"] = &Evaluator::EvaluateAssignExpr;
	table["exprSemicolon"] = &Evaluator::EvaluateExprSemicolon;
	table["ifstmt"] = &Evaluator::EvaluateIfStmt;
	table["if_elsestmt"] = &Evaluator::EvaluateIfElseStmt;
	table["whilestmt"] = &Evaluator::EvaluateWhileStmt;
	table["forstmt"] = &Evaluator::EvaluateForStmt;
	table["return"] = &Evaluator::EvaluateReturnStmt;
	table["return_value"] = &Evaluator::EvaluateReturnValueStmt;
	table["break"] = &Evaluator::EvaluateBreak;
	table["continue"] = &Evaluator::EvaluateContinue;
	table["semicolon"] = &Evaluator::EvaluateSemicolon;
	table["elist"] = &Evaluator::EvaluateElist;
	table["emptyElist"] = &Evaluator::EvaluateEmptyElist;
	table["indexed"] = &Evaluator::EvaluateIndexed;
	table["indexedElem"] = &Evaluator::EvaluateIndexedElem;
	table["elistObjectdef"] = &Evaluator::EvaluateElistObjectdef;
	table["indexedObjectdef"] = &Evaluator::EvaluateIndexedObjectdef;
	table["block"] = &Evaluator::EvaluateBlock;
	table["funcdef"] = &Evaluator::EvaluateFuncdef;
	table["anonymousFuncdef"] = &Evaluator::EvaluateAnonymousFuncdef;
	table["funcEnter"] = &Evaluator::EvaluateFuncEnter;
	table["funcbody"] = &Evaluator::EvaluateFuncBody;
	table["numConst"] = &Evaluator::EvaluateNumberConst;
	table["stringConst"] = &Evaluator::EvaluateStringConst;
	table["boolConst"] = &Evaluator::EvaluateBoolConst;
	table["nil"] = &Evaluator::EvaluateNIL;
	table["param"] = &Evaluator::EvaluateParam;
	table["optionalParam"] = &Evaluator::EvaluateOptionalParam;
	table["idlist"] = &Evaluator::EvaluateIdlist;
	table["emptyIdlist"] = &Evaluator::EvaluateEmptyIdlist;
	table["print"] = &Evaluator::EvaluatePrint;
	table["typeof"] = &Evaluator::EvaluateTypeof;
	table["object_keys"] = &Evaluator::EvaluateObject_keys;
	table["object_size"] = &Evaluator::EvaluateObject_size;
	table["eval"] = &Evaluator::EvaluateEval;
	table["meta_syntax"] = &Evaluator::EvaluateSyntax;
	table["meta_escape"] = &Evaluator::EvaluateEscape;
	table["meta_execute"] = &Evaluator::EvaluateExecute;
	table["meta_parse"] = &Evaluator::EvaluateParse;
	table["meta_unparse"] = &Evaluator::EvaluateUnparse;
	table["metaAST"] = &Evaluator::EvaluateMetaAST;

	return table;
}

std::map<std::string, Value& (Evaluator::*)(ASTnode*, bool, Environment*)> Evaluator::IntializeLvalueDispatcher() {
	std::map<std::string, Value& (Evaluator::*)(ASTnode*, bool, Environment*)> table;

	table["var"] = &Evaluator::EvaluateLvalueIdent;
	table["localVar"] = &Evaluator::EvaluateLvalueLocalIdent;
	table["globalVar"] = &Evaluator::EvaluateLvalueGlobalIdent;
	table["member_lvalueVar"] = &Evaluator::EvaluateLvalueMemberIdent;
	table["member_lvalueBrackets"] = &Evaluator::EvaluateLvalueMemberBrackets;
	table["member_callVar"] = &Evaluator::EvaluateLvalueMemberCallIdent;
	table["member_callBrackets"] = &Evaluator::EvaluateLvalueMemberCallBrackets;

	return table;
}

Evaluator::Evaluator() {
	EvaluateDispatcher = IntializeDispatcher();
	EvaluateLvalueDispatcher = IntializeLvalueDispatcher();
}

Evaluator* Evaluator::getInstance() {
	if (!evaluator)
		evaluator = new Evaluator();
	return evaluator;
}

void Evaluator::destroyInstance(){
	assert(evaluator);
	delete evaluator;
}

OPValue Evaluator::Evaluate(ASTnode* node, bool insertFlag) {
	lineNumber = node->GetLine();
	OPValue ret = (this->*EvaluateDispatcher[node->GetValue("type")->GetStringValue()])(node, insertFlag);
	return ret;
}

Value& Evaluator::EvaluateLvalue(ASTnode* node, bool insertFlag, Environment* env) {
	lineNumber = node->GetLine();
	Value& ret = (this->*EvaluateLvalueDispatcher[node->GetValue("type")->GetStringValue()])(node,insertFlag, env);
	return ret;
}

OPValue Evaluator::EvaluateProgram(ASTnode* node, bool insertFlag) {
	OPValue tmp;
	double numOfStmt = node->GetValue("numOfStmt")->GetNumberValue();
	for (int i = 0; i < numOfStmt; i++) {
		tmp = Evaluate(node->GetValue(std::to_string(i))->GetObjectValue());
	}
	return std::nullopt;
}

// expr
OPValue Evaluator::EvaluateAddExpr(ASTnode* node, bool insertFlag) {
	OPValue left = Evaluate(node->GetValue("left")->GetObjectValue());
	OPValue right = Evaluate(node->GetValue("right")->GetObjectValue());
	return *left + *right;
}

OPValue Evaluator::EvaluateSubExpr(ASTnode* node, bool insertFlag) {
	OPValue left = Evaluate(node->GetValue("left")->GetObjectValue());
	OPValue right = Evaluate(node->GetValue("right")->GetObjectValue());
	return *left - *right;
}

OPValue Evaluator::EvaluateMulExpr(ASTnode* node, bool insertFlag) {
	OPValue left = Evaluate(node->GetValue("left")->GetObjectValue());
	OPValue right = Evaluate(node->GetValue("right")->GetObjectValue());
	return *left * *right;
}

OPValue Evaluator::EvaluateDivExpr(ASTnode* node, bool insertFlag) {
	OPValue left = Evaluate(node->GetValue("left")->GetObjectValue());
	OPValue right = Evaluate(node->GetValue("right")->GetObjectValue());
	return *left / *right;
}

OPValue Evaluator::EvaluateModExpr(ASTnode* node, bool insertFlag) {
	OPValue left = Evaluate(node->GetValue("left")->GetObjectValue());
	OPValue right = Evaluate(node->GetValue("right")->GetObjectValue());
	return *left % *right;
}

OPValue Evaluator::EvaluateGreaterExpr(ASTnode* node, bool insertFlag) {
	OPValue left = Evaluate(node->GetValue("left")->GetObjectValue());
	OPValue right = Evaluate(node->GetValue("right")->GetObjectValue());
	return *left > *right;
}

OPValue Evaluator::EvaluateGreaterOrEqualExpr(ASTnode* node, bool insertFlag) {
	OPValue left = Evaluate(node->GetValue("left")->GetObjectValue());
	OPValue right = Evaluate(node->GetValue("right")->GetObjectValue());
	return *left >= *right;
}

OPValue Evaluator::EvaluateLessExpr(ASTnode* node, bool insertFlag) {
	OPValue left = Evaluate(node->GetValue("left")->GetObjectValue());
	OPValue right = Evaluate(node->GetValue("right")->GetObjectValue());
	return *left < *right;
}

OPValue Evaluator::EvaluateLessOrEqualExpr(ASTnode* node, bool insertFlag) {
	OPValue left = Evaluate(node->GetValue("left")->GetObjectValue());
	OPValue right = Evaluate(node->GetValue("right")->GetObjectValue());
	return *left <= *right;
}

OPValue Evaluator::EvaluateEqualExpr(ASTnode* node, bool insertFlag) {
	OPValue left = Evaluate(node->GetValue("left")->GetObjectValue());
	OPValue right = Evaluate(node->GetValue("right")->GetObjectValue());
	return *left == *right;
}

OPValue Evaluator::EvaluateNotEqualExpr(ASTnode* node, bool insertFlag) {
	OPValue left = Evaluate(node->GetValue("left")->GetObjectValue());
	OPValue right = Evaluate(node->GetValue("right")->GetObjectValue());
	return *left != *right;
}

OPValue Evaluator::EvaluateAndExpr(ASTnode* node, bool insertFlag) {
	OPValue left = Evaluate(node->GetValue("left")->GetObjectValue());
	OPValue right = Evaluate(node->GetValue("right")->GetObjectValue());
	return *left && *right;
}

OPValue Evaluator::EvaluateOrExpr(ASTnode* node, bool insertFlag) {
	OPValue left = Evaluate(node->GetValue("left")->GetObjectValue());
	OPValue right = Evaluate(node->GetValue("right")->GetObjectValue());
	return *left || *right;
}


// term
OPValue Evaluator::EvaluateParenthesis(ASTnode* node, bool insertFlag) {
	return Evaluate(node->GetValue("expr")->GetObjectValue());
}

OPValue Evaluator::EvaluateUminus(ASTnode* node, bool insertFlag) {
	return -(*Evaluate(node->GetValue("expr")->GetObjectValue()));
}

OPValue Evaluator::EvaluateNot(ASTnode* node, bool insertFlag) {
	return !(*Evaluate(node->GetValue("expr")->GetObjectValue()));
}


OPValue Evaluator::EvaluatePreIncrement(ASTnode* node, bool insertFlag) {
	Value& lvalue = EvaluateLvalue(node->GetValue("lvalue")->GetObjectValue(),false);
	return ++lvalue;
}

OPValue Evaluator::EvaluatePostIncrement(ASTnode* node, bool insertFlag) {
	Value& lvalue = EvaluateLvalue(node->GetValue("lvalue")->GetObjectValue(), false);
	return lvalue++;
}

OPValue Evaluator::EvaluatePreDecrement(ASTnode* node, bool insertFlag) {
	Value& lvalue = EvaluateLvalue(node->GetValue("lvalue")->GetObjectValue(), false);
	return --lvalue;
}

OPValue Evaluator::EvaluatePostDecrement(ASTnode* node, bool insertFlag) {
	Value& lvalue = EvaluateLvalue(node->GetValue("lvalue")->GetObjectValue(), false);
	return lvalue--;
}


// primary
OPValue Evaluator::EvaluateParenthesisFuncdef(ASTnode* node, bool insertFlag) {
//	return Evaluate(node->GetValue("funcdef")->GetObjectValue()); 
	ExpressionfunctionDefinition(node);
	// Error checking
	Object* idList = node->GetValue("funcEnter")->GetObjectValue()->GetValue("idlist")->GetObjectValue();
	double numOfParams = idList->GetValue("numOfParams")->GetNumberValue();

	Object idList_withoutIndex;
	for (double i = 0; i < numOfParams; ++i) {
		Object* obj = idList->GetValue(i)->GetObjectValue();
		Value id = *(obj->GetValue("ID"));
		if (idList_withoutIndex.HasProperty(id))
			throw RuntimeErrorException("More than one formal with name \"" + id.GetStringValue() + "\"");
		if (hasCollisionWithLibFunc(id.GetStringValue()))
			throw RuntimeErrorException("\"" + id.GetStringValue() + "\" is a library function. It can not be used as a formal");
		idList_withoutIndex.Set(id, Undefined());
	}

	return (Object *)node;

}

//lvalue
Value& Evaluator::EvaluateLvalueIdent(ASTnode* node, bool insertFlag, Environment* env) {
	Value* value = LvalueVarActions(node->GetValue("ID")->GetStringValue(),insertFlag, env);
	if (value == nullptr && !insertFlag) {
		throw RuntimeErrorException("Undefined variable \"" + node->GetValue("ID")->GetStringValue() + "\"");
	}
	else if (value == nullptr && insertFlag) {
		throw SyntaxErrorException("Collision with library function: \"" + node->GetValue("ID")->GetStringValue() + "\"");
	}
	return *value;
}

Value& Evaluator::EvaluateLvalueLocalIdent(ASTnode* node, bool insertFlag, Environment* env) {
	Value* value = LvalueLocalVarActions(node->GetValue("ID")->GetStringValue(), insertFlag, env);
	if (value == nullptr && !insertFlag) {
		throw RuntimeErrorException("Undefined local variable \"" + node->GetValue("ID")->GetStringValue() + "\"");
	}
	else if (value == nullptr && insertFlag) {
		throw SyntaxErrorException("Collision with library function: \"" + node->GetValue("ID")->GetStringValue() + "\"");
	}
	return *value;
}

Value& Evaluator::EvaluateLvalueGlobalIdent(ASTnode* node, bool insertFlag, Environment* env) {
	Value* value = GlobalVarActions(node->GetValue("ID")->GetStringValue(), env);
	if (value == nullptr) {
		throw RuntimeErrorException("Undefined global variable \"" + node->GetValue("ID")->GetStringValue() + "\"");
	}
	return *value;
}

//rvalue
OPValue Evaluator::EvaluateIdent(ASTnode* node, bool insertFlag) {
	Value* value = RvalueVarActions(node->GetValue("ID")->GetStringValue(), insertFlag);
	if (value == nullptr) {
		throw RuntimeErrorException("Undefined variable \"" + node->GetValue("ID")->GetStringValue() + "\"");
	}
	return *value;
}

OPValue Evaluator::EvaluateLocalIdent(ASTnode* node, bool insertFlag) {
	Value* value = RvalueLocalVarActions(node->GetValue("ID")->GetStringValue());
	if (value == nullptr && insertFlag) {
		throw SyntaxErrorException("Collision with library function \"" + node->GetValue("ID")->GetStringValue() + "\"");
	}
	else if (value == nullptr && !insertFlag) {
		throw RuntimeErrorException("Undefined or collision with \"" + node->GetValue("ID")->GetStringValue() + "\"");
	}
	return *value;
}

OPValue Evaluator::EvaluateGlobalIdent(ASTnode* node, bool insertFlag) {
	Value* value = GlobalVarActions(node->GetValue("ID")->GetStringValue());
	if (value == nullptr) {
		throw RuntimeErrorException("Undefined global variable \"" + node->GetValue("ID")->GetStringValue() + "\"");
	}
	return *value;
}


//lvalue member
Value& Evaluator::EvaluateLvalueMemberIdent(ASTnode* node, bool insertFlag, Environment* env )
{
	Value& lvalue = EvaluateLvalue(node->GetValue("lvalue")->GetObjectValue(),false);
	if (lvalue.isUndefined()) throw RuntimeErrorException("Cannot read id \"" + node->GetValue("ID")->GetStringValue() + "\" of undefined");
	if (lvalue.isObject()) {
		return Object_set(lvalue, node->GetValue("ID")->GetStringValue());
	}
	throw RuntimeErrorException("Cannot read id \"" + node->GetValue("ID")->GetStringValue() + "\" of non-object variable");
}

Value& Evaluator::EvaluateLvalueMemberBrackets(ASTnode* node, bool insertFlag, Environment* env)
{
	OPValue expr = Evaluate(node->GetValue("expr")->GetObjectValue(), false);
	if (*expr == nil) throw RuntimeErrorException("Cannot use nil as expression in brackets");
	//std::cout << "\n-------------------------------------------\n";
	//std::cout << *expr << std::endl;
	Value& lvalue = EvaluateLvalue(node->GetValue("lvalue")->GetObjectValue(), false);
	//std::cout << *expr << std::endl;
	//std::cout << "\n-------------------------------------------\n";
	//std::cout << lvalue;
	if (lvalue.isUndefined()) throw RuntimeErrorException("Cannot read value \"" + expr->toString() + "\" of undefined");
	if (lvalue.isObject()) {
		return Object_set_brackets(lvalue, *expr);
	}
	throw RuntimeErrorException("Cannot read value \"" + expr->toString() + "\" of non-object variable");
}

Value& Evaluator::EvaluateLvalueMemberCallIdent(ASTnode* node, bool insertFlag, Environment* env){
	OPValue call = Evaluate(node->GetValue("call")->GetObjectValue(), false);
	if (call->isUndefined()) throw RuntimeErrorException("Cannot read id \"" + node->GetValue("ID")->GetStringValue() + "\" of undefined");
	if (call->isObject()) {
		return Object_set(*call, node->GetValue("ID")->GetStringValue());
	}
	throw RuntimeErrorException("Cannot read id \"" + node->GetValue("ID")->GetStringValue() + "\" of non-object variable");
}

Value& Evaluator::EvaluateLvalueMemberCallBrackets(ASTnode* node, bool insertFlag, Environment* env){
	OPValue expr = Evaluate(node->GetValue("expr")->GetObjectValue(), false);
	if (*expr == nil) throw RuntimeErrorException("Cannot use nil as expression in brackets");
	Value& call = EvaluateLvalue(node->GetValue("call")->GetObjectValue(), false);
	if (call.isUndefined()) throw RuntimeErrorException("Cannot read value \"" + expr->toString() + "\" of undefined");
	if (call.isObject()) {
		return Object_set_brackets(call, *expr);
	}
	throw RuntimeErrorException("Cannot read value \"" + expr->toString() + "\" of non-object variable");
}


//rvalue member
OPValue Evaluator::EvaluateMemberIdent(ASTnode* node, bool insertFlag) {
	OPValue rvalue = Evaluate(node->GetValue("lvalue")->GetObjectValue(), false);
	if (rvalue->isUndefined()) throw RuntimeErrorException("Cannot read id \"" + node->GetValue("ID")->GetStringValue() +"\" of undefined");
	if (rvalue->isObject()) {
		Value* value = Object_get(*rvalue, node->GetValue("ID")->GetStringValue());
		if (value != nullptr)
			return *value;
	}
	return Undefined();
}

OPValue Evaluator::EvaluateMemberBrackets(ASTnode* node, bool insertFlag) {
	OPValue expr = Evaluate(node->GetValue("expr")->GetObjectValue(), false);
	if (*expr == nil) throw RuntimeErrorException("Cannot use nil as expression in brackets");
	OPValue rvalue = Evaluate(node->GetValue("lvalue")->GetObjectValue(), false);
	if (rvalue->isUndefined()) throw RuntimeErrorException("Cannot read value \"" + expr->toString() + "\" of undefined");
	if (rvalue->isObject()) {
		Value* value = Object_get_brackets(*rvalue, *expr);
		if (value != nullptr)
			return *value;
	}
	return Undefined();
}

OPValue Evaluator::EvaluateMemberCallIdent(ASTnode* node, bool insertFlag) {
	OPValue call = Evaluate(node->GetValue("call")->GetObjectValue(), false);
	if (call->isUndefined()) throw RuntimeErrorException("Cannot read id \"" + node->GetValue("ID")->GetStringValue() + "\" of undefined");
	if (call->isObject()) {
		Value* value = Object_get(*call, node->GetValue("ID")->GetStringValue());
		if (value != nullptr)
			return *value;
	}
	return Undefined();
}

OPValue Evaluator::EvaluateMemberCallBrackets(ASTnode* node, bool insertFlag) {
	OPValue expr = Evaluate(node->GetValue("expr")->GetObjectValue(), false);
	if (*expr == nil) throw RuntimeErrorException("Cannot use nil as expression in brackets");
	OPValue call = Evaluate(node->GetValue("call")->GetObjectValue(), false);
	if (call->isUndefined()) throw RuntimeErrorException("Cannot read value \"" + expr->toString() + "\" of undefined");
	if (call->isObject()) {
		Value* value = Object_get_brackets(*call, *expr);
		if (value != nullptr)
			return *value;
	}
	return Undefined();
}



OPValue Evaluator::EvaluateMultiCall(ASTnode* node, bool insertFlag) {
	Environment* oldCurrent = TemporarilySaveEnvironment(EnvironmentHolder::getInstance()->GetCurrentEnv());
	Object* old_argTable = argTable;

	OPValue tmp = Evaluate(node->GetValue("argList")->GetObjectValue(), false);
	OPValue funcdef = Evaluate(node->GetValue("call")->GetObjectValue(), false);
	
	funcdef = CallerEnvironmentActions(*funcdef);

	OPValue retValue = Evaluate(funcdef->GetObjectValue()->GetValue("funcEnter")->GetObjectValue());
	DeleteArgTable();
	argTable = old_argTable;
	LeaveFunctionEnvironment(oldCurrent);
	DecreaseTemporarilySavedEnvironment(oldCurrent);
	return retValue;
}

OPValue Evaluator::EvaluateLvalueNormalCall(ASTnode* node, bool insertFlag){
	Environment* oldCurrent = TemporarilySaveEnvironment(EnvironmentHolder::getInstance()->GetCurrentEnv());
	Object* old_argTable = argTable;
	OPValue retValue;
	OPValue tmp = Evaluate(node->GetValue("argList")->GetObjectValue(), false);
	OPValue funcdef = Evaluate(node->GetValue("lvalue")->GetObjectValue(), false);

	if (!(funcdef->isObject() && funcdef->GetObjectValue()) && !(funcdef->isString() && hasCollisionWithLibFunc(funcdef->GetStringValue())))
		throw RuntimeErrorException("Cannot call a not callable-member");
	if (funcdef->isString()) {
		retValue = Evaluate(GlobalLookUp(funcdef->GetStringValue())->GetObjectValue());
		DeleteArgTable();
		argTable = old_argTable;
	}
	else if (funcdef->GetObjectValue()->HasProperty("type") && hasCollisionWithLibFunc(funcdef->GetObjectValue()->GetValue("type")->GetStringValue())) {		// libfunc
		retValue = Evaluate(funcdef->GetObjectValue());
		DeleteArgTable();
		argTable = old_argTable;
	}
	else {
		funcdef = CallerEnvironmentActions(*funcdef);
		retValue = Evaluate(funcdef->GetObjectValue()->GetValue("funcEnter")->GetObjectValue());
		DeleteArgTable();
		argTable = old_argTable;
		LeaveFunctionEnvironment(oldCurrent);
	}
	DecreaseTemporarilySavedEnvironment(oldCurrent);
	return retValue;
}

OPValue Evaluator::EvaluateLvalueMethodCall(ASTnode* node, bool insertFlag) {
	Environment* oldCurrent = TemporarilySaveEnvironment(EnvironmentHolder::getInstance()->GetCurrentEnv());

	Object* old_argTable = argTable; 

	OPValue tmp = Evaluate(node->GetValue("argList")->GetObjectValue(), false);
	OPValue lvalue = Evaluate(node->GetValue("lvalue")->GetObjectValue());
	
	if (!lvalue->isObject() || lvalue->GetObjectValue() == nil || lvalue->GetObjectValue()->HasProperty("$closure")) {
		throw RuntimeErrorException("Left hand is not an Object in method call");
	}
	
	AddLvalueAsFirstArg(*lvalue);

	Value* funcdef = Object_get(*lvalue, node->GetValue("ID")->GetStringValue());
	*funcdef = CallerEnvironmentActions(*funcdef);
	
	OPValue retValue = Evaluate(funcdef->GetObjectValue()->GetValue("funcEnter")->GetObjectValue());
	DeleteArgTable();
	argTable = old_argTable;
	LeaveFunctionEnvironment(oldCurrent);	
	DecreaseTemporarilySavedEnvironment(oldCurrent);
	return retValue;
}

OPValue Evaluator::EvaluateFuncdefCall(ASTnode* node, bool insertFlag) {
	Environment* oldCurrent = TemporarilySaveEnvironment(EnvironmentHolder::getInstance()->GetCurrentEnv());

	Object* old_argTable = argTable;

	OPValue tmp = Evaluate(node->GetValue("argList")->GetObjectValue(), false);
	OPValue funcdef = Evaluate(node->GetValue("funcdef")->GetObjectValue(), false);
	funcdef = CallerEnvironmentActions(*funcdef);

	OPValue retValue = Evaluate(funcdef->GetObjectValue()->GetValue("funcEnter")->GetObjectValue());
	DeleteArgTable();
	argTable = old_argTable;
	LeaveFunctionEnvironment(oldCurrent);
	DecreaseTemporarilySavedEnvironment(oldCurrent);

	return retValue;
}

//arglist
OPValue Evaluator::EvaluateArglist(ASTnode* node, bool insertFlag) {
	Object* myArgTable = new Object();
	double numOfTotalArgs =  node->GetValue("numOfTotalArgs")->GetNumberValue();
	double numOfPositionalArgs = node->GetValue("numOfPositionalArgs")->GetNumberValue();
	Object* PositionalArgs = node->GetValue("PositionalArgs")->GetObjectValue();
	Object* NamedArgs = node->GetValue("NamedArgs")->GetObjectValue();
	Object* myPositionalArgs = new Object();
	Object* myNamedArgs = new Object();

	myArgTable->Set("numOfTotalArgs", numOfTotalArgs);
	myArgTable->Set("numOfPositionalArgs", numOfPositionalArgs);
	//myArgTable->Set("PositionalArgs", new Object());
	//myArgTable->Set("NamedArgs", new Object());

	for (double i = 0; i < numOfPositionalArgs; ++i) {
		//myArgTable->GetValue("PositionalArgs")->GetObjectValue()->Set(i, *Evaluate(PositionalArgs->GetValue(i)->GetObjectValue()));
		myPositionalArgs->Set(i, *Evaluate(PositionalArgs->GetValue(i)->GetObjectValue()));
	}
	for (auto kv : NamedArgs->GetMap()) {
		//myArgTable->GetValue("NamedArgs")->GetObjectValue()->Set(kv.first, *Evaluate(kv.second.GetObjectValue()));
		myNamedArgs->Set(kv.first, *Evaluate(kv.second.GetObjectValue()));
	}

	myArgTable->Set("PositionalArgs", myPositionalArgs);
	myArgTable->Set("NamedArgs", myNamedArgs);

	argTable = myArgTable;
	return std::nullopt;
}

OPValue Evaluator::EvaluateEmptyArglist(ASTnode* node, bool insertFlag) {
	argTable = new Object();
	argTable->Set("numOfTotalArgs", 0.0);
	argTable->Set("numOfPositionalArgs", 0.0);
	argTable->Set("PositionalArgs",  (Object*)nullptr);													
	argTable->Set("NamedArgs", (Object*)nullptr);														
	return std::nullopt;
}




OPValue interpreter::Evaluator::EvaluateAssignExpr(ASTnode* node, bool insertFlag)
{
	Environment* savedEnvironment = TemporarilySaveEnvironment(EnvironmentHolder::getInstance()->GetCurrentEnv());
	
	OPValue expr =  Evaluate(node->GetValue("expr")->GetObjectValue());
	Value& lvalue = EvaluateLvalue(node->GetValue("lvalue")->GetObjectValue(), true, savedEnvironment);

	if (lvalue.isObject() && lvalue.GetObjectValue())
		lvalue.GetObjectValue()->DecreaseReferenceCounter();

	if (expr->isObject() && expr->GetObjectValue())
		expr->GetObjectValue()->IncreaseReferenceCounter();

	DecreaseTemporarilySavedEnvironment(savedEnvironment);

	lvalue = *expr;
	return lvalue;
}

// stmt

OPValue Evaluator::EvaluateExprSemicolon(ASTnode* node, bool insertFlag) {
	return Evaluate(node->GetValue("expr")->GetObjectValue());
}

OPValue Evaluator::EvaluateIfStmt(ASTnode* node, bool insertFlag) {
	OPValue tmp;
	if (Evaluate(node->GetValue("condition")->GetObjectValue())->toBool()) {
		tmp = Evaluate(node->GetValue("stmt")->GetObjectValue()); 
		return true;
	}
	return false;
}

OPValue Evaluator::EvaluateIfElseStmt(ASTnode* node, bool insertFlag) {
	OPValue tmp;
	if (!Evaluate(node->GetValue("ifstmt")->GetObjectValue())->toBool()) {
		tmp = Evaluate(node->GetValue("elsestmt")->GetObjectValue());
	}
	return std::nullopt;
}

OPValue Evaluator::EvaluateWhileStmt(ASTnode* node, bool insertFlag) {
	OPValue tmp;
	while (Evaluate(node->GetValue("condition")->GetObjectValue())->toBool()) {
		try { tmp = Evaluate(node->GetValue("stmt")->GetObjectValue()); }
		catch (BreakException& ) { break; }
		catch (ContinueException& ) { continue; }
	}
	return std::nullopt;
}

OPValue Evaluator::EvaluateForStmt(ASTnode* node, bool insertFlag) {
	OPValue tmp,elist,init_elist;
	for (init_elist = Evaluate(node->GetValue("init_elist")->GetObjectValue());
		Evaluate(node->GetValue("condition")->GetObjectValue())->toBool();
		elist = Evaluate(node->GetValue("elist")->GetObjectValue())) {
		try { tmp = Evaluate(node->GetValue("stmt")->GetObjectValue()); }
		catch (BreakException& ) {break;}
		catch (ContinueException& ) { 
			if(elist->isObject() && elist->GetObjectValue())
				DeleteForElist(elist->GetObjectValue());
			continue; 
			}
		if(elist->isObject() && elist->GetObjectValue())
			DeleteForElist(elist->GetObjectValue());
	}
	if (elist->isObject() && elist->GetObjectValue())
		DeleteForElist(elist->GetObjectValue());
	DeleteForElist(init_elist->GetObjectValue());	
	//init_elist->GetObjectValue()->DecreaseReferenceCounter();
	//elist->GetObjectValue()->DecreaseReferenceCounter();
	return std::nullopt;
}

OPValue Evaluator::EvaluateReturnStmt(ASTnode* node, bool insertFlag) {
	throw ReturnException();
}

OPValue Evaluator::EvaluateReturnValueStmt(ASTnode* node, bool insertFlag) {
	Value oldRetVal = retVal;
	if (retFlag == false) {
		retFlag = true;
		retVal = *Evaluate(node->GetValue("expr")->GetObjectValue(), false);
		if (oldRetVal.isObject() && oldRetVal.GetObjectValue())
			oldRetVal.GetObjectValue()->DecreaseReferenceCounter();
		retFlag = false;
	}
	else
	{
		retVal = *Evaluate(node->GetValue("expr")->GetObjectValue(), false);
	}
	if (retVal.isObject() && retVal.GetObjectValue())
		retVal.GetObjectValue()->IncreaseReferenceCounter();
	throw ReturnValueException();
}

OPValue Evaluator::EvaluateBreak(ASTnode* node, bool insertFlag)
{
	throw BreakException();
}

OPValue Evaluator::EvaluateContinue(ASTnode* node, bool insertFlag)
{
	throw ContinueException();
}

OPValue Evaluator::EvaluateSemicolon(ASTnode* node, bool insertFlag) { return std::nullopt; }



//elist
OPValue Evaluator::EvaluateElist(ASTnode* node, bool insertFlag) {
	Object* elistMap = new Object();
	double numOfExprs = node->GetValue("numOfExprs")->GetNumberValue();
	for (int i = 0; i < numOfExprs; i++) {
		elistMap->Set((double)i, *Evaluate(node->GetValue(std::to_string(i))->GetObjectValue()));
	}
	return elistMap;
}

OPValue Evaluator::EvaluateEmptyElist(ASTnode* node, bool insertFlag) {
	return new Object();
}

//indexed
OPValue Evaluator::EvaluateIndexed(ASTnode* node, bool insertFlag) {
	Object* indexedMap = new Object();
	double numOfElems = node->GetValue("numOfElems")->GetNumberValue();
	for (int i = 0; i < numOfElems; i++) {
		Object* obj = Evaluate(node->GetValue(std::to_string(i))->GetObjectValue())->GetObjectValue();
		for (auto kv : obj->GetMap()) {
			indexedMap->Set(kv.first, kv.second);
		}
		//delete obj;
		DeleteForElist(obj);
	}
	return indexedMap;
}

//indexedElem
OPValue Evaluator::EvaluateIndexedElem(ASTnode* node, bool insertFlag) {
	Object* newObject = new Object();
	OPValue key = Evaluate(node->GetValue("keyExpr")->GetObjectValue());
	OPValue value = Evaluate(node->GetValue("valueExpr")->GetObjectValue());
	if (key->isUndefined()) throw RuntimeErrorException("Key of value \"" + value->toString() + "\" is undefined");
	newObject->Set(*key, *value);
	return newObject;
}


//object
OPValue Evaluator::EvaluateElistObjectdef(ASTnode* node, bool insertFlag) {
	return Evaluate(node->GetValue("elist")->GetObjectValue());
}

OPValue Evaluator::EvaluateIndexedObjectdef(ASTnode* node, bool insertFlag) {
	return Evaluate(node->GetValue("indexed")->GetObjectValue());
}


//block
OPValue Evaluator::EvaluateBlock(ASTnode* node, bool insertFlag) {
	CreateBlockEnvironment();

	OPValue tmp;
	double numOfStmt = node->GetValue("numOfStmt")->GetNumberValue();
	for (int i = 0; i < numOfStmt; i++) {
		tmp = Evaluate(node->GetValue(std::to_string(i))->GetObjectValue());
	}

	LeaveBlockEnvironment();
	return std::nullopt;
}

OPValue interpreter::Evaluator::EvaluateFuncdef(ASTnode* node, bool insertFlag)
{
	Value* value = LvalueFuncDefActions(node->GetValue("ID")->GetStringValue(),node);
	if (value == nullptr) {
		throw RuntimeErrorException("Function with id \"" + node->GetValue("ID")->GetStringValue() + "\" already exists");
	}

	// Error checking
	Object* idList = node->GetValue("funcEnter")->GetObjectValue()->GetValue("idlist")->GetObjectValue();
	double numOfParams = idList->GetValue("numOfParams")->GetNumberValue();

	Object idList_withoutIndex;
	for (double i = 0; i < numOfParams; ++i) {
		Object* obj = idList->GetValue(i)->GetObjectValue();
		Value id = *(obj->GetValue("ID"));
		if (idList_withoutIndex.HasProperty(id))
			throw RuntimeErrorException("More than one formal with name \"" + id.GetStringValue() + "\"");
		if(hasCollisionWithLibFunc(id.GetStringValue()))
			throw RuntimeErrorException("\"" + id.GetStringValue() + "\" is a library function. It can not be used as a formal");
		idList_withoutIndex.Set(id, Undefined());
	}

	return *value;
}

OPValue interpreter::Evaluator::EvaluateAnonymousFuncdef(ASTnode* node, bool insertFlag)
{
	return *InsertFunctionDefinition(Object::GenerateAnonymousName(), node);
}


OPValue Evaluator::EvaluateFuncEnter(ASTnode* node, bool insertFlag) {
	Object* idList = (*Evaluate(node->GetValue("idlist")->GetObjectValue(), false)).GetObjectValue();
	Object* PositionalArgs = argTable->GetValue("PositionalArgs")->GetObjectValue();
	Object* NamedArgs = argTable->GetValue("NamedArgs")->GetObjectValue();
	double numOfParams = idList->GetValue("numOfParams")->GetNumberValue();
	double numOfTotalArgs = argTable->GetValue("numOfTotalArgs")->GetNumberValue();
	double numOfPositionalArgs = argTable->GetValue("numOfPositionalArgs")->GetNumberValue();

	if (argTable == nullptr) assert(false);
	if (numOfTotalArgs > numOfParams) throw RuntimeErrorException("More actual arguments than function parameters"); 


	Object idList_withoutIndex;
	for (double i = 0; i < numOfParams; ++i) {
		Object* obj = idList->GetValue(i)->GetObjectValue();
		Value id = *(obj->GetValue("ID"));
		if (obj->HasProperty("expr"))
			idList_withoutIndex.Set(id, *(obj->GetValue("expr")));
		else
			idList_withoutIndex.Set(id, (Object*)nullptr);
	}
	if(PositionalArgs)
		AddPositionalParamsToEnvironment(idList);
	if(NamedArgs)
		AddNamedParamsToEnvironment(idList_withoutIndex);

	// Default Params
	if (numOfTotalArgs < numOfParams) {
		for (auto kv : idList_withoutIndex.GetMap()) {
			std::string id = kv.first.GetStringValue();
			Object* expr = kv.second.GetObjectValue();
			if (!LocalLookUp(id)) {
				if (expr) {
					Environment* curr = TemporarilySaveEnvironment(EnvironmentHolder::getInstance()->GetCurrentEnv());
					curr->Set(id, *Evaluate(expr));
					DecreaseTemporarilySavedEnvironment(curr);
				}
				else {
					throw RuntimeErrorException("Too few arguments in funciton call. Parameter \"" + id + "\" has no value (also not default value is defined)");
				}
			}
		}
	}

	
	try { OPValue tmp = Evaluate(node->GetValue("funcbody")->GetObjectValue()); }
	catch (ReturnException&) { return Undefined(); }
	catch (ReturnValueException&) { return retVal; }

	return Undefined();
}

OPValue Evaluator::EvaluateFuncBody(ASTnode* node, bool insertFlag) {
	OPValue tmp;
	double numOfStmt = node->GetValue("numOfStmt")->GetNumberValue();
	for (int i = 0; i < numOfStmt; i++) {
		tmp = Evaluate(node->GetValue(std::to_string(i))->GetObjectValue());
	}
	return std::nullopt;
}

// const
OPValue Evaluator::EvaluateNumberConst(ASTnode* node, bool insertFlag) {
	return node->GetValue("value")->GetNumberValue();
}

OPValue Evaluator::EvaluateStringConst(ASTnode* node, bool insertFlag) {
	return node->GetValue("value")->GetStringValue();
}

OPValue Evaluator::EvaluateBoolConst(ASTnode* node, bool insertFlag) {
	return node->GetValue("value")->GetBoolValue();
}

OPValue Evaluator::EvaluateNIL(ASTnode* node, bool insertFlag) {
	return (Object*) nullptr;
}


//formal
OPValue Evaluator::EvaluateParam(ASTnode* node, bool insertFlag) {
	Object* obj = new Object();
	obj->Set(node->GetValue("ID")->GetStringValue(),Undefined());
	return obj;
}

OPValue Evaluator::EvaluateOptionalParam(ASTnode* node, bool insertFlag) {
	Object* obj = new Object();
	obj->Set(node->GetValue("ID")->GetStringValue(), *Evaluate(node->GetValue("expr")->GetObjectValue(), false));
	return obj;
}

//idlist
OPValue Evaluator::EvaluateIdlist(ASTnode* node, bool insertFlag) { 
	return node; 
}

OPValue Evaluator::EvaluateEmptyIdlist(ASTnode* node, bool insertFlag) {
	return node;
}


OPValue Evaluator::EvaluatePrint(ASTnode* node, bool insertFlag) {
	Object* PositionalArgs = argTable->GetValue("PositionalArgs")->GetObjectValue();
	double numOfTotalArgs = argTable->GetValue("numOfTotalArgs")->GetNumberValue();
	double numOfPositionalArgs = argTable->GetValue("numOfPositionalArgs")->GetNumberValue();
	if (numOfTotalArgs - numOfPositionalArgs != 0) throw SyntaxErrorException("Cannot use named argument in library function print()");
	for (double i = 0; i < numOfPositionalArgs; ++i) {
		std::cout << *(PositionalArgs->GetValue(i));	
	}
	return Undefined();
}

OPValue Evaluator::EvaluateTypeof(ASTnode* node, bool insertFlag) {
	Object* PositionalArgs = argTable->GetValue("PositionalArgs")->GetObjectValue();
	double numOfTotalArgs = argTable->GetValue("numOfTotalArgs")->GetNumberValue();
	double numOfPositionalArgs = argTable->GetValue("numOfPositionalArgs")->GetNumberValue();
	if (numOfTotalArgs - numOfPositionalArgs != 0) throw RuntimeErrorException("Cannot use named argument in library function typeof()");

	if (numOfTotalArgs != 1)
		throw RuntimeErrorException("Size of arguments in library function typeof must be equal to 1");

	Value argument = *(PositionalArgs->GetValue(0.0));

	if (argument.isObject()) {
		if (argument.GetObjectValue() == nullptr)
			return "nil";
		else if (argument.GetObjectValue()->HasProperty("type")
			&& hasCollisionWithLibFunc(argument.GetObjectValue()->GetValue("type")->GetStringValue()))
			return "libfunc";
		else if (argument.GetObjectValue()->HasProperty("$closure"))
			return "userfunc";
		return "object";
	}
	else if (argument.isBool())
		return "boolean";
	else if (argument.isNumber())
		return "number";
	else if (argument.isString())
		return "string";
	else if (argument.isValueReference())
		return "native value reference";
	else if (argument.isUndefined())
		return "undefined";
}

OPValue Evaluator::EvaluateObject_keys(ASTnode* node, bool insertFlag) {
	Object* PositionalArgs = argTable->GetValue("PositionalArgs")->GetObjectValue();
	double numOfTotalArgs = argTable->GetValue("numOfTotalArgs")->GetNumberValue();
	double numOfPositionalArgs = argTable->GetValue("numOfPositionalArgs")->GetNumberValue();
	if (numOfTotalArgs - numOfPositionalArgs != 0) throw SyntaxErrorException("Cannot use named argument in library function typeof()");
	Value argument = *(PositionalArgs->GetValue(0.0));

	if (numOfTotalArgs != 1)
		throw RuntimeErrorException("Size of arguments in library function object_keys must be equal to 1");
	if (!argument.isObject() || argument.GetObjectValue() == nil || argument.GetObjectValue()->HasProperty("$closure"))
		throw RuntimeErrorException("Argument of library function object_keys is not object");
	
	Object* returnKeys = new Object();
	double i = 0;

	for (auto kv : argument.GetObjectValue()->GetMap()) {
		returnKeys->Set(i, kv.first);
		i++;
	}

	return returnKeys;
}

OPValue Evaluator::EvaluateObject_size(ASTnode* node, bool insertFlag) {
	Object* PositionalArgs = argTable->GetValue("PositionalArgs")->GetObjectValue();
	double numOfTotalArgs = argTable->GetValue("numOfTotalArgs")->GetNumberValue();
	double numOfPositionalArgs = argTable->GetValue("numOfPositionalArgs")->GetNumberValue();
	if (numOfTotalArgs - numOfPositionalArgs != 0) throw SyntaxErrorException("Cannot use named argument in library function typeof()");
	Value argument = *(PositionalArgs->GetValue(0.0));

	if (numOfTotalArgs != 1)
		throw RuntimeErrorException("Size of arguments in library function object_keys must be equal to 1");
	if (!argument.isObject() || argument.GetObjectValue() == nil || argument.GetObjectValue()->HasProperty("$closure"))
		throw RuntimeErrorException("Argument of library function object_keys is not object");

	return (double)argument.GetObjectValue()->size();
}

OPValue Evaluator::EvaluateEval(ASTnode* node, bool insertFlag) {
	double numOfTotalArgs = argTable->GetValue("numOfTotalArgs")->GetNumberValue();
	double numOfPositionalArgs = argTable->GetValue("numOfPositionalArgs")->GetNumberValue();
	if (numOfTotalArgs != 1 || numOfPositionalArgs != numOfTotalArgs) throw SyntaxErrorException("Too many argument in library function eval()");
	
	Value* arg = argTable->GetValue("PositionalArgs")->GetObjectValue()->GetValue(0.0);
	if(!arg->isString()) throw RuntimeErrorException("Library function eval() takes only string valua as an argument");
	std::string arg_s = arg->GetStringValue();
	
	yyscan_t scanner;
	yylex_init(&scanner);
	yy_scan_string(arg_s.c_str(), scanner);
	AST* ast = new AST();
	yyparse(ast, scanner, 1);
	OPValue tmp = Evaluator::getInstance()->Evaluate(ast->GetRoot());
	yylex_destroy(scanner);
	if (ast->GetRoot())
		ast->GetRoot()->DecreaseReferenceCounter();
	
	return Undefined();
}

// metaprogramming
bool inSyntax = false;

OPValue  Evaluator::EvaluateSyntax(ASTnode* node, bool insertFlag) {
	Value* ast = node->GetValue("expr");
	assert(isAST(*ast));
	ASTnode* metaAst = new  ASTnode("type","metaAST");
	bool old_inSyntax = inSyntax;
	inSyntax = true;
	metaAst->Set("root", SyntaxParseOuter(ast->GetObjectValue()));
	inSyntax = old_inSyntax;
	return metaAst; 
}

OPValue  Evaluator::EvaluateEscape(ASTnode* node, bool insertFlag) {
	if (!inSyntax) throw RuntimeErrorException("Meta tag Escape out of Syntax");
	OPValue ast = Evaluate(node->GetValue("meta_var")->GetObjectValue());
	if (!isAST(*ast)) throw RuntimeErrorException("Not AST input for meta tag Escape");
	return ast;
}

OPValue  Evaluator::EvaluateExecute(ASTnode* node, bool insertFlag) {
	OPValue ast = Evaluate(node->GetValue("expr")->GetObjectValue());
	if (!isAST(*ast)) throw RuntimeErrorException("Can not execute right value. It is not an AST");
	ast->GetObjectValue()->IncreaseReferenceCounter();
	OPValue ret = Evaluator::getInstance()->Evaluate(ast->GetObjectValue()->GetValue("root")->GetObjectValue());
	ast->GetObjectValue()->DecreaseReferenceCounter();
	return ret;
}


OPValue  Evaluator::EvaluateParse(ASTnode* node, bool insertFlag) {
	yyscan_t scanner;
	yylex_init(&scanner);
	yy_scan_string(node->GetValue("stringConst")->GetStringValue().c_str(), scanner);
	
	AST* ast = new AST();
	yyparse(ast, scanner, 2);

	ASTnode* metaAST = new ASTnode("type", "metaAST");
	metaAST->Set("root", ast->GetRoot());

	ast->GetRoot()->DecreaseReferenceCounter();
	delete ast;
	yylex_destroy(scanner);

	return metaAST;
}

OPValue  Evaluator::EvaluateUnparse(ASTnode* node, bool insertFlag) {
	ASTnode* ast = node->GetValue("expr")->GetObjectValue();
	if (!isAST(ast)) throw RuntimeErrorException(" Can not Unparse right value. It is not an AST");
	return ast->toString();
}

OPValue  Evaluator::EvaluateMetaAST(ASTnode* node, bool insertFlag) {
	return node; 
}
