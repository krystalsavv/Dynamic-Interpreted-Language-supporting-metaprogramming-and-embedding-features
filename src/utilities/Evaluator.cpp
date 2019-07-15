#include "utilities/Evaluator.h"
#define nil (Object *)nullptr

using namespace interpreter;

static Object* argTable = nullptr;

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
	//table["multiCall"] = &Evaluator::EvaluateMultiCall;
	table["lvalueCall"] = &Evaluator::EvaluateLvalueCallSuffix;
	//table["funcdefCall"] = &Evaluator::EvaluateFuncdefCall;
	table["normcall"] = &Evaluator::EvaluateNormCall;
	//table["methodcall"] = &Evaluator::EvaluateMethodCall;
	table["argList"] = &Evaluator::EvaluateArglist;
	table["emptyArgList"] = &Evaluator::EvaluateEmptyArglist;
	table["assignexpr"] = &Evaluator::EvaluateAssignExpr;
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

OPValue Evaluator::Evaluate(ASTnode* node, bool insertFlag) {
	return (this->*EvaluateDispatcher[node->GetValue("type")->GetStringValue()])(node, insertFlag);
}

Value& Evaluator::EvaluateLvalue(ASTnode* node, bool insertFlag, Environment* env) {
	return (this->*EvaluateLvalueDispatcher[node->GetValue("type")->GetStringValue()])(node,insertFlag, env);
}

OPValue Evaluator::EvaluateProgram(ASTnode* node, bool insertFlag) {
	InitGlobalEnvironment();

	OPValue tmp;
	double numOfStmt = node->GetValue("numOfStmt")->GetNumberValue();
	for (int i = 0; i < numOfStmt; i++) {
		try { tmp = Evaluate(node->GetValue(std::to_string(i))->GetObjectValue()); }
		catch (BreakException& e) {std::cout << std::endl << e.what() << std::endl;exit(0);}
		catch (ContinueException& e) { std::cout << std::endl << e.what() << std::endl; exit(0); }
		catch (ReturnException& e) { std::cout << std::endl << e.what() << std::endl; exit(0); }
		catch (ReturnValueException& e) { std::cout << std::endl << e.what() << std::endl; exit(0); }
		catch (RuntimeErrorException& e) { std::cout << std::endl << e.what() << std::endl; exit(0); }
		catch (SyntaxErrorException& e) { std::cout << std::endl << e.what() << std::endl; exit(0); }
		
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

OPValue Evaluator::EvaluateEqualExpr(ASTnode* node, bool insertFlag) {						// TODO: na elenxoume to object 
	OPValue left = Evaluate(node->GetValue("left")->GetObjectValue());
	OPValue right = Evaluate(node->GetValue("right")->GetObjectValue());
	return *left == *right;
}

OPValue Evaluator::EvaluateNotEqualExpr(ASTnode* node, bool insertFlag) {					// TODO: na elenxoume to object 
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
	return Evaluate(node->GetValue("funcdef")->GetObjectValue()); 
}

//lvalue
Value& Evaluator::EvaluateLvalueIdent(ASTnode* node, bool insertFlag, Environment* env) {
	Value* value = LvalueVarActions(node->GetValue("ID")->GetStringValue(),insertFlag, env);
	if (value == nullptr && !insertFlag) {
		throw RuntimeErrorException("Undefined variable " + node->GetValue("ID")->GetStringValue());
	}
	else if (value == nullptr && insertFlag) {
		throw SyntaxErrorException("Collision with library function: " + node->GetValue("ID")->GetStringValue());
	}
	return *value;
}

Value& Evaluator::EvaluateLvalueLocalIdent(ASTnode* node, bool insertFlag, Environment* env) {
	Value* value = LvalueLocalVarActions(node->GetValue("ID")->GetStringValue(), insertFlag, env);
	if (value == nullptr && !insertFlag) {
		throw RuntimeErrorException("Undefined local variable " + node->GetValue("ID")->GetStringValue());
	}
	else if (value == nullptr && insertFlag) {
		throw SyntaxErrorException("Collision with library function: " + node->GetValue("ID")->GetStringValue());
	}
	return *value;
}

Value& Evaluator::EvaluateLvalueGlobalIdent(ASTnode* node, bool insertFlag, Environment* env) {
	Value* value = GlobalVarActions(node->GetValue("ID")->GetStringValue(), env);
	if (value == nullptr) {
		throw RuntimeErrorException("Undefined global variable " + node->GetValue("ID")->GetStringValue());
	}
	return *value;
}

//rvalue
OPValue Evaluator::EvaluateIdent(ASTnode* node, bool insertFlag) {
	Value* value = RvalueVarActions(node->GetValue("ID")->GetStringValue(), insertFlag);
	if (value == nullptr) {
		throw RuntimeErrorException("Undefined variable " + node->GetValue("ID")->GetStringValue());
	}
	return *value;
}

OPValue Evaluator::EvaluateLocalIdent(ASTnode* node, bool insertFlag) {
	Value* value = RvalueLocalVarActions(node->GetValue("ID")->GetStringValue());
	if (value == nullptr && insertFlag) {
		throw SyntaxErrorException("Collision with library function " + node->GetValue("ID")->GetStringValue());
	}
	else if (value == nullptr && !insertFlag) {
		throw RuntimeErrorException("Undefined or collision with" + node->GetValue("ID")->GetStringValue());
	}
	return *value;
}

OPValue Evaluator::EvaluateGlobalIdent(ASTnode* node, bool insertFlag) {
	Value* value = GlobalVarActions(node->GetValue("ID")->GetStringValue());
	if (value == nullptr) {
		throw RuntimeErrorException("Undefined global variable " + node->GetValue("ID")->GetStringValue());
	}
	return *value;
}


//lvalue member
Value& Evaluator::EvaluateLvalueMemberIdent(ASTnode* node, bool insertFlag, Environment* env )
{
	Value& lvalue = EvaluateLvalue(node->GetValue("lvalue")->GetObjectValue(),false);
	if (lvalue.isUndefined()) throw RuntimeErrorException("Cannot read id " + node->GetValue("ID")->GetStringValue() + " of undefined");
	if (lvalue.isObject()) {
		return Object_set(lvalue, node->GetValue("ID")->GetStringValue());
	}
	throw RuntimeErrorException("Cannot read id " + node->GetValue("ID")->GetStringValue() + " of non-object variable");
}

Value& Evaluator::EvaluateLvalueMemberBrackets(ASTnode* node, bool insertFlag, Environment* env)
{
	OPValue expr = Evaluate(node->GetValue("expr")->GetObjectValue(), false);
	if (*expr == nil) throw RuntimeErrorException("Cannot use nil as expression in brackets");
	Value& lvalue = EvaluateLvalue(node->GetValue("lvalue")->GetObjectValue(), false);
	if (lvalue.isUndefined()) throw RuntimeErrorException("Cannot read value " + expr->toString() + " of undefined");
	if (lvalue.isObject()) {
		return Object_set_brackets(lvalue, *expr);
	}
	throw RuntimeErrorException("Cannot read value " + expr->toString() + " of non-object variable");
}

Value& Evaluator::EvaluateLvalueMemberCallIdent(ASTnode* node, bool insertFlag, Environment* env){
	OPValue call = Evaluate(node->GetValue("call")->GetObjectValue(), false);
	if (call->isUndefined()) throw RuntimeErrorException("Cannot read id " + node->GetValue("ID")->GetStringValue() + " of undefined");
	if (call->isObject()) {
		return Object_set(*call, node->GetValue("ID")->GetStringValue());
	}
	throw RuntimeErrorException("Cannot read id " + node->GetValue("ID")->GetStringValue() + " of non-object variable");
}

Value& Evaluator::EvaluateLvalueMemberCallBrackets(ASTnode* node, bool insertFlag, Environment* env){
	OPValue expr = Evaluate(node->GetValue("expr")->GetObjectValue(), false);
	if (*expr == nil) throw RuntimeErrorException("Cannot use nil as expression in brackets");
	Value& call = EvaluateLvalue(node->GetValue("call")->GetObjectValue(), false);
	if (call.isUndefined()) throw RuntimeErrorException("Cannot read value " + expr->toString() + " of undefined");
	if (call.isObject()) {
		return Object_set_brackets(call, *expr);
	}
	throw RuntimeErrorException("Cannot read value " + expr->toString() + " of non-object variable");
}


//rvalue member
OPValue Evaluator::EvaluateMemberIdent(ASTnode* node, bool insertFlag) {
	OPValue rvalue = Evaluate(node->GetValue("lvalue")->GetObjectValue(), false);
	if (rvalue->isUndefined()) throw RuntimeErrorException("Cannot read id " + node->GetValue("ID")->GetStringValue() +" of undefined");
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
	if (rvalue->isUndefined()) throw RuntimeErrorException("Cannot read value " + expr->toString() + " of undefined");
	if (rvalue->isObject()) {
		Value* value = Object_get_brackets(*rvalue, *expr);
		if (value != nullptr)
			return *value;
	}
	return Undefined();
}

OPValue Evaluator::EvaluateMemberCallIdent(ASTnode* node, bool insertFlag) {
	OPValue call = Evaluate(node->GetValue("call")->GetObjectValue(), false);
	if (call->isUndefined()) throw RuntimeErrorException("Cannot read id " + node->GetValue("ID")->GetStringValue() + " of undefined");
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
	if (call->isUndefined()) throw RuntimeErrorException("Cannot read value " + expr->toString() + " of undefined");
	if (call->isObject()) {
		Value* value = Object_get_brackets(*call, *expr);
		if (value != nullptr)
			return *value;
	}
	return Undefined();
}


OPValue Evaluator::EvaluateLvalueCallSuffix(ASTnode* node, bool insertFlag){
	Environment* oldCurrent = EnvironmentHolder::getInstance()->GetCurrentEnv();

	// TODO: na tsekarw pio prin gia libfunc (alliws tha petaksei error h evalute ths lvalue)

	OPValue funcdef = Evaluate(node->GetValue("lvalue")->GetObjectValue(), false);
	CallerEnvironmentActions(*funcdef);

	OPValue tmp = Evaluate(node->GetValue("argList")->GetObjectValue(), false);
	OPValue retValue = Evaluate(funcdef->GetObjectValue()->GetValue("funcEnter")->GetObjectValue());
	LeaveFunctionEnvironment(oldCurrent);
	return retValue;
}



//normcall
OPValue Evaluator::EvaluateNormCall(ASTnode* node, bool insertFlag) {
	OPValue tmp = Evaluate(node->GetValue("argList")->GetObjectValue());
	return std::nullopt;
}

//arg
OPValue Evaluator::EvaluateArg(ASTnode* node, bool insertFlag) {
	return Value();
}

//arglist
OPValue Evaluator::EvaluateArglist(ASTnode* node, bool insertFlag) {
	// free an eixe proigoymeno object an den to kanw otan teleiwsw na to use
	argTable = new Object(*node);
	double numOfTotalArgs =  argTable->GetValue("numOfTotalArgs")->GetNumberValue();
	double numOfPositionalArgs = argTable->GetValue("numOfPositionalArgs")->GetNumberValue();
	Object* PositionalArgs = argTable->GetValue("PositionalArgs")->GetObjectValue();
	Object* NamedArgs = argTable->GetValue("NamedArgs")->GetObjectValue();
	for (double i = 0; i < numOfPositionalArgs; ++i) {
		PositionalArgs->Set(i, *Evaluate(PositionalArgs->GetValue(i)->GetObjectValue()));
	}
	for (auto kv : NamedArgs->GetMap()) {
		NamedArgs->Set(kv.first, *Evaluate(kv.second.GetObjectValue()));
	}
	return std::nullopt;
}

OPValue Evaluator::EvaluateEmptyArglist(ASTnode* node, bool insertFlag) {
	Object* emptyArgList = new Object();
	emptyArgList->Set("numOfTotalArgs", 0.0);
	emptyArgList->Set("numOfPositionalArgs", 0.0);
	return emptyArgList;
}

OPValue interpreter::Evaluator::EvaluateAssignExpr(ASTnode* node, bool insertFlag)
{
	Environment* savedEnvironment = EnvironmentHolder::getInstance()->GetCurrentEnv();
	OPValue expr =  Evaluate(node->GetValue("expr")->GetObjectValue());
	Value& lvalue = EvaluateLvalue(node->GetValue("lvalue")->GetObjectValue(), savedEnvironment);
	if (lvalue == nil) throw RuntimeErrorException("Cannot be used nil as lvalue");
	lvalue = *expr;
	return lvalue;
}

// stmt
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
	OPValue tmp;
	for (tmp = Evaluate(node->GetValue("init_elist")->GetObjectValue());
		Evaluate(node->GetValue("condition")->GetObjectValue())->toBool();
		tmp = Evaluate(node->GetValue("elist")->GetObjectValue())) {
		try { tmp = Evaluate(node->GetValue("stmt")->GetObjectValue()); }
		catch (BreakException& ) { break; }
		catch (ContinueException& ) { continue; }
	}
	return std::nullopt;
}

OPValue Evaluator::EvaluateReturnStmt(ASTnode* node, bool insertFlag) {
	throw ReturnException();
}

OPValue Evaluator::EvaluateReturnValueStmt(ASTnode* node, bool insertFlag) {
	retVal = *Evaluate(node->GetValue("expr")->GetObjectValue());
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
		obj->~Object(); // call destructor to free this object (TODO: decrise reference counter )
	}
	return indexedMap;
}

//indexedElem
OPValue Evaluator::EvaluateIndexedElem(ASTnode* node, bool insertFlag) {
	Object* newObject = new Object();
	OPValue key = Evaluate(node->GetValue("keyExpr")->GetObjectValue());
	OPValue value = Evaluate(node->GetValue("valueExpr")->GetObjectValue());
	if (key->isUndefined()) throw RuntimeErrorException("Key of value " + value->toString() + " is undefined");
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
		throw SyntaxErrorException("Function with id " + node->GetValue("ID")->GetStringValue() + " already exists");
	}


	/*Object* idList = node->GetValue("funcEnter")->GetObjectValue()->GetValue("idlist")->GetObjectValue();
	double numOfParams = idList->GetValue("numOfParams")->GetNumberValue();

	Object idList_withoutIndex;
	for (double i = 0; i < numOfParams; ++i) {
		Object* obj = idList->GetValue(i)->GetObjectValue();
		Value id = *(obj->GetValue("ID"));
		if (idList_withoutIndex.HasProperty(id)) {
			throw RuntimeErrorException("More than one formal with name " + id.GetStringValue());
		}
		idList_withoutIndex.Set(id, (Object*)nullptr);
	}*/

	return *value;
}

OPValue interpreter::Evaluator::EvaluateAnonymousFuncdef(ASTnode* node, bool insertFlag)
{
	return InsertFunctionDefinition(Object::GenerateAnonymousName(), node);										// HERE RETURN TRUE
}


OPValue Evaluator::EvaluateFuncEnter(ASTnode* node, bool insertFlag) {
	Object* idList = (*Evaluate(node->GetValue("idlist")->GetObjectValue(), false)).GetObjectValue();
	Object* PositionalArgs = argTable->GetValue("PositionalArgs")->GetObjectValue();
	Object* NamedArgs = argTable->GetValue("NamedArgs")->GetObjectValue();
	double numOfParams = idList->GetValue("numOfParams")->GetNumberValue();
	double numOfTotalArgs = argTable->GetValue("numOfTotalArgs")->GetNumberValue();
	double numOfPositionalArgs = argTable->GetValue("numOfPositionalArgs")->GetNumberValue();

	if (argTable == nullptr) assert(false);
	if (numOfTotalArgs > numOfParams) throw RuntimeErrorException("More actual arguments than function paramiters"); 

	Object idList_withoutIndex;
	for (double i = 0; i < numOfParams; ++i) {
		Object* obj = idList->GetValue(i)->GetObjectValue();
		Value id = *(obj->GetValue("ID"));
		if (idList_withoutIndex.HasProperty(id)) {
			throw RuntimeErrorException("More than one formal with name " + id.GetStringValue());
		}
		if (obj->HasProperty("expr"))
			idList_withoutIndex.Set(id, *(obj->GetValue("expr")));
		else
			idList_withoutIndex.Set(id, (Object*)nullptr);
	}
	AddPositionalParamsToEnvironment(idList, argTable);
	AddNamedParamsToEnvironment(idList_withoutIndex, argTable);

	// Default Params
	if (numOfTotalArgs < numOfParams) {
		for (auto kv : idList_withoutIndex.GetMap()) {
			std::string id = kv.first.GetStringValue();
			Object* expr = kv.second.GetObjectValue();
			if (!LocalLookUp(id)) {
				if (expr) {
					Environment* curr = EnvironmentHolder::getInstance()->GetCurrentEnv();
					curr->Set(id, *Evaluate(expr));
				}
				else {
					throw RuntimeErrorException("Too few arguments in funciton call. Paramiter " + id + " has no value (also not default value is defined)");
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
	// TODO: Extra error checking ensuring that all default arguments are at the end of the parameter list after any required parameters 
	return new Object(*node);
}

OPValue Evaluator::EvaluateEmptyIdlist(ASTnode* node, bool insertFlag) {
	return new Object("numOfParams", 0.0);
}


OPValue Evaluator::EvaluatePrint(ASTnode* node, bool insertFlag) {
	//object of arguments in node ??
	for (int i = 0; i < node->size(); i++) {
		std::cout << node->GetValue(std::to_string(i));
		if (i != node->size() - 1)
			std::cout << ",";
	}
	return std::nullopt;
}

OPValue Evaluator::EvaluateTypeof(ASTnode* node, bool insertFlag) {
	//object of arguments in node ??
	if (node->size() != 1)
		throw SyntaxErrorException("Invalid arguments in typeof");
	if (node->GetValue("0")->isBool())
		return "boolean";
	else if (node->GetValue("0")->isNumber())
		return "number";
	else if (node->GetValue("0")->isObject())
		return "object";
	else if (node->GetValue("0")->isString())
		return "string";
	else if (node->GetValue("0")->isUndefined())
		return "undefined";
	return std::nullopt;
}

OPValue Evaluator::EvaluateObject_keys(ASTnode* node, bool insertFlag) {
	//object of arguments in node ??
	if (node->size() != 1 || !node->GetValue("0")->isObject())
		throw SyntaxErrorException("Invalid arguments in object_keys");
	Object* returnKeys = new Object();
	Object* argument = node->GetValue("0")->GetObjectValue();
	for (int i = 0; i < argument->size(); i++)
	{
		returnKeys->Set(std::to_string(i), argument->GetValue(std::to_string(i)));
	}
	return returnKeys;
}

OPValue Evaluator::EvaluateObject_size(ASTnode* node, bool insertFlag) {
	//object of arguments in node ??
	if (node->size() != 1 || !node->GetValue("0")->isObject())
		throw SyntaxErrorException("Invalid arguments in object_size");
	return (double)node->GetValue("0")->GetObjectValue()->size();
}

OPValue Evaluator::EvaluateEval(ASTnode* node, bool insertFlag) {
	return std::nullopt;
}