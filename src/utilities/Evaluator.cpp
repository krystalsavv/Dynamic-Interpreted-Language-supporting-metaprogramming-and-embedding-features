#include "utilities/Evaluator.h"
#define nil (Object *)nullptr

using namespace interpreter;

std::map<std::string, std::optional<Value>(Evaluator::*)(ASTnode*,bool)> Evaluator::IntializeDispatcher() {
	std::map<std::string, std::optional<Value>(Evaluator::*)(ASTnode*, bool insertFlag)> table;

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
	//table["member_callVar"] = &Evaluator::EvaluateMemberCallIdent;
	//table["member_callBrackets"] = &Evaluator::EvaluateMemberCallBrackets;
	//table["multiCall"] = &Evaluator::EvaluateMultiCall;
	//table["lvalueCall"] = &Evaluator::EvaluateLvalueCallSuffix;
	//table["funcdefCall"] = &Evaluator::EvaluateFuncdefCall;
	table["normcall"] = &Evaluator::EvaluateNormCall;
	//table["methodcall"] = &Evaluator::EvaluateMethodCall;
	//table["namedParam"] = &Evaluator::EvaluateArg;
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
	table["numConst"] = &Evaluator::EvaluateNumberConst;
	table["stringConst"] = &Evaluator::EvaluateStringConst;
	table["boolConst"] = &Evaluator::EvaluateBoolConst;
	table["nil"] = &Evaluator::EvaluateNIL;
	//table["param"] = &Evaluator::EvaluateParam;
	//table["optionalParam"] = &Evaluator::EvaluateOptionalParam;
	table["idlist"] = &Evaluator::EvaluateIdlist;
	table["emptyIdlist"] = &Evaluator::EvaluateEmptyIdlist;
	table["print"] = &Evaluator::EvaluatePrint;
	table["typeof"] = &Evaluator::EvaluateTypeof;
	table["object_keys"] = &Evaluator::EvaluateObject_keys;
	table["object_size"] = &Evaluator::EvaluateObject_size;
	table["eval"] = &Evaluator::EvaluateEval;

	return table;
}

std::map<std::string, std::optional<std::reference_wrapper<Value>>(Evaluator::*)(ASTnode*, bool, Environment*)> Evaluator::IntializeLvalueDispatcher() {
	std::map<std::string, std::optional<std::reference_wrapper<Value>>(Evaluator::*)(ASTnode*, bool, Environment*)> table;

	table["var"] = &Evaluator::EvaluateLvalueIdent;
	table["localVar"] = &Evaluator::EvaluateLvalueLocalIdent;
	table["globalVar"] = &Evaluator::EvaluateLvalueGlobalIdent;
	table["member_lvalueVar"] = &Evaluator::EvaluateLvalueMemberIdent;
	//table["member_lvalueBrackets"] = &Evaluator::EvaluateLvalueMemberBrackets;
	//table["member_callVar"] = &Evaluator::EvaluateLvalueMemberCallIdent;
	//table["member_callBrackets"] = &Evaluator::EvaluateLvalueMemberCallBrackets;

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

std::optional<Value> Evaluator::Evaluate(ASTnode* node, bool insertFlag) {
	return (this->*EvaluateDispatcher[node->GetValue("type")->GetStringValue()])(node, insertFlag);
}

std::optional<std::reference_wrapper<Value>> Evaluator::EvaluateLvalue(ASTnode* node, bool insertFlag, Environment* env) {
	return (this->*EvaluateLvalueDispatcher[node->GetValue("type")->GetStringValue()])(node,insertFlag, env);
}

std::optional<Value> Evaluator::EvaluateProgram(ASTnode* node, bool insertFlag) {
	InitGlobalEnvironment();

	Value tmp;
	double numOfStmt = node->GetValue("numOfStmt")->GetNumberValue();
	for (int i = 0; i < numOfStmt; i++) {
		try { tmp = *Evaluate(node->GetValue(std::to_string(i))->GetObjectValue()); }
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
std::optional<Value> Evaluator::EvaluateAddExpr(ASTnode* node, bool insertFlag) {
	Value left = *Evaluate(node->GetValue("left")->GetObjectValue());
	Value right = *Evaluate(node->GetValue("right")->GetObjectValue());
	return left + right;
}

std::optional<Value> Evaluator::EvaluateSubExpr(ASTnode* node, bool insertFlag) {
	Value left = *Evaluate(node->GetValue("left")->GetObjectValue());
	Value right = *Evaluate(node->GetValue("right")->GetObjectValue());
	return left - right;
}

std::optional<Value> Evaluator::EvaluateMulExpr(ASTnode* node, bool insertFlag) {
	Value left = *Evaluate(node->GetValue("left")->GetObjectValue());
	Value right = *Evaluate(node->GetValue("right")->GetObjectValue());
	return left * right;
}

std::optional<Value> Evaluator::EvaluateDivExpr(ASTnode* node, bool insertFlag) {
	Value left = *Evaluate(node->GetValue("left")->GetObjectValue());
	Value right = *Evaluate(node->GetValue("right")->GetObjectValue());
	return left / right;
}

std::optional<Value> Evaluator::EvaluateModExpr(ASTnode* node, bool insertFlag) {
	Value left = *Evaluate(node->GetValue("left")->GetObjectValue());
	Value right = *Evaluate(node->GetValue("right")->GetObjectValue());
	return left % right;
}

std::optional<Value> Evaluator::EvaluateGreaterExpr(ASTnode* node, bool insertFlag) {
	Value left = *Evaluate(node->GetValue("left")->GetObjectValue());
	Value right = *Evaluate(node->GetValue("right")->GetObjectValue());
	return left > right;
}

std::optional<Value> Evaluator::EvaluateGreaterOrEqualExpr(ASTnode* node, bool insertFlag) {
	Value left = *Evaluate(node->GetValue("left")->GetObjectValue());
	Value right = *Evaluate(node->GetValue("right")->GetObjectValue());
	return left >= right;
}

std::optional<Value> Evaluator::EvaluateLessExpr(ASTnode* node, bool insertFlag) {
	Value left = *Evaluate(node->GetValue("left")->GetObjectValue());
	Value right = *Evaluate(node->GetValue("right")->GetObjectValue());
	return left < right;
}

std::optional<Value> Evaluator::EvaluateLessOrEqualExpr(ASTnode* node, bool insertFlag) {
	Value left = *Evaluate(node->GetValue("left")->GetObjectValue());
	Value right = *Evaluate(node->GetValue("right")->GetObjectValue());
	return left <= right;
}

std::optional<Value> Evaluator::EvaluateEqualExpr(ASTnode* node, bool insertFlag) {						// TODO: na elenxoume to object 
	Value left = *Evaluate(node->GetValue("left")->GetObjectValue());
	Value right = *Evaluate(node->GetValue("right")->GetObjectValue());
	return left == right;
}

std::optional<Value> Evaluator::EvaluateNotEqualExpr(ASTnode* node, bool insertFlag) {					// TODO: na elenxoume to object 
	Value left = *Evaluate(node->GetValue("left")->GetObjectValue());
	Value right = *Evaluate(node->GetValue("right")->GetObjectValue());
	return left != right;
}

std::optional<Value> Evaluator::EvaluateAndExpr(ASTnode* node, bool insertFlag) {
	Value left = *Evaluate(node->GetValue("left")->GetObjectValue());
	Value right = *Evaluate(node->GetValue("right")->GetObjectValue());
	return left && right;
}

std::optional<Value> Evaluator::EvaluateOrExpr(ASTnode* node, bool insertFlag) {
	Value left = *Evaluate(node->GetValue("left")->GetObjectValue());
	Value right = *Evaluate(node->GetValue("right")->GetObjectValue());
	return left || right;
}


// term
std::optional<Value> Evaluator::EvaluateParenthesis(ASTnode* node, bool insertFlag) {
	return Evaluate(node->GetValue("expr")->GetObjectValue());
}

std::optional<Value> Evaluator::EvaluateUminus(ASTnode* node, bool insertFlag) {
	return (*Evaluate(node->GetValue("expr")->GetObjectValue()) * Value(-1.0));
}

std::optional<Value> Evaluator::EvaluateNot(ASTnode* node, bool insertFlag) {
	return !(*Evaluate(node->GetValue("expr")->GetObjectValue()));
}


std::optional<Value> Evaluator::EvaluatePreIncrement(ASTnode* node, bool insertFlag) {
	Value& lvalue = *EvaluateLvalue(node->GetValue("lvalue")->GetObjectValue(),false);
	return ++lvalue;
}

std::optional<Value> Evaluator::EvaluatePostIncrement(ASTnode* node, bool insertFlag) {
	Value& lvalue = *EvaluateLvalue(node->GetValue("lvalue")->GetObjectValue(), false);
	return lvalue++;
}

std::optional<Value> Evaluator::EvaluatePreDecrement(ASTnode* node, bool insertFlag) {
	Value& lvalue = *EvaluateLvalue(node->GetValue("lvalue")->GetObjectValue(), false);
	return --lvalue;
}

std::optional<Value> Evaluator::EvaluatePostDecrement(ASTnode* node, bool insertFlag) {
	Value& lvalue = *EvaluateLvalue(node->GetValue("lvalue")->GetObjectValue(), false);
	return lvalue--;
}


// primary
std::optional<Value> Evaluator::EvaluateParenthesisFuncdef(ASTnode* node, bool insertFlag) {
	return Evaluate(node->GetValue("funcdef")->GetObjectValue()); 
}

//lvalue
std::optional<std::reference_wrapper<Value>> Evaluator::EvaluateLvalueIdent(ASTnode* node, bool insertFlag, Environment* env) {
	Value* value = LvalueVarActions(node->GetValue("ID")->GetStringValue(),insertFlag, env);
	if (value == nullptr && !insertFlag) {
		throw RuntimeErrorException("Cannot find variable " + node->GetValue("ID")->GetStringValue());
	}
	else if (value == nullptr && insertFlag) {
		throw SyntaxErrorException("Collision with library function: " + node->GetValue("ID")->GetStringValue());
	}
	return *value;
}

std::optional<std::reference_wrapper<Value>> Evaluator::EvaluateLvalueLocalIdent(ASTnode* node, bool insertFlag, Environment* env) {
	Value* value = LvalueLocalVarActions(node->GetValue("ID")->GetStringValue(), insertFlag, env);
	if (value == nullptr && !insertFlag) {
		throw RuntimeErrorException("Cannot find local variable " + node->GetValue("ID")->GetStringValue());
	}
	else if (value == nullptr && insertFlag) {
		throw SyntaxErrorException("Collision with library function: " + node->GetValue("ID")->GetStringValue());
	}
	return *value;
}

std::optional<std::reference_wrapper<Value>> Evaluator::EvaluateLvalueGlobalIdent(ASTnode* node, bool insertFlag, Environment* env) {
	Value* value = GlobalVarActions(node->GetValue("ID")->GetStringValue(), env);
	if (value == nullptr) {
		throw RuntimeErrorException();
	}
	return *value;
}

//rvalue
std::optional<Value> Evaluator::EvaluateIdent(ASTnode* node, bool insertFlag) {
	Value* value = RvalueVarActions(node->GetValue("ID")->GetStringValue(), insertFlag);
	if (value == nullptr) {
		throw RuntimeErrorException("Undefined " + node->GetValue("ID")->GetStringValue());
	}
	return *value;
}

std::optional<Value> Evaluator::EvaluateLocalIdent(ASTnode* node, bool insertFlag) {
	Value* value = RvalueLocalVarActions(node->GetValue("ID")->GetStringValue());
	if (value == nullptr && insertFlag) {
		throw SyntaxErrorException("Collision with library function " + node->GetValue("ID")->GetStringValue());
	}
	else if (value == nullptr && !insertFlag) {
		throw RuntimeErrorException("Undefined or collision with" + node->GetValue("ID")->GetStringValue());
	}
	return *value;
}

std::optional<Value> Evaluator::EvaluateGlobalIdent(ASTnode* node, bool insertFlag) {
	Value* value = GlobalVarActions(node->GetValue("ID")->GetStringValue());
	if (value == nullptr) {
		throw RuntimeErrorException();
	}
	return *value;
}

//lvalue member
std::optional<std::reference_wrapper<Value>> Evaluator::EvaluateLvalueMemberIdent(ASTnode* node, bool insertFlag, Environment* env )
{
	Value& rvalue = *EvaluateLvalue(node->GetValue("lvalue")->GetObjectValue(),false);
	if (rvalue.isUndefined()) throw RuntimeErrorException("Cannot read symbol " + node->GetValue("ID")->GetStringValue() + " of undefined");
	if (rvalue.isObject()) {
		for (int i = 0; i < rvalue.GetObjectValue()->size(); i++) {
			if (rvalue.GetObjectValue()->GetValue(std::to_string(i))->isObject()
				&& rvalue.GetObjectValue()->GetValue(std::to_string(i))->GetObjectValue()->HasProperty(node->GetValue("ID")->GetStringValue())) {
				return *rvalue.GetObjectValue()->GetValue(std::to_string(i))->GetObjectValue()->GetValue(node->GetValue("ID")->GetStringValue());
			}
		}
		Object* value = new Object();
		value->Set(node->GetValue("ID")->GetStringValue(),Undefined());
		int index = rvalue.GetObjectValue()->size();
		rvalue.GetObjectValue()->Set(std::to_string(index),value);
		return *rvalue.GetObjectValue()->GetValue(std::to_string(index))->GetObjectValue()->GetValue(node->GetValue("ID")->GetStringValue());
	}
	throw RuntimeErrorException("Cannot read symbol " + node->GetValue("ID")->GetStringValue() + " of non-object variable");
}

//std::optional<std::reference_wrapper<Value>> Evaluator::EvaluateLvalueMemberBrackets(ASTnode* node) {
//
//}
//
//std::optional<std::reference_wrapper<Value>> Evaluator::EvaluateLvalueMemberCallIdent(ASTnode* node) {
//
//}
//
//std::optional<std::reference_wrapper<Value>> Evaluator::EvaluateLvalueMemberCallBrackets(ASTnode* node) {
//
//}
//

//rvalue member
std::optional<Value> Evaluator::EvaluateMemberIdent(ASTnode* node, bool insertFlag) {
	Value rvalue = *Evaluate(node->GetValue("lvalue")->GetObjectValue(), false);
	if (rvalue.isUndefined()) throw RuntimeErrorException("Cannot read symbol " + node->GetValue("ID")->GetStringValue() +" of undefined");
	if (rvalue.isObject()) {
		for (int i = 0; i < rvalue.GetObjectValue()->size(); i++) {
			if ( rvalue.GetObjectValue()->GetValue(std::to_string(i))->isObject() 
				&& rvalue.GetObjectValue()->GetValue(std::to_string(i))->GetObjectValue()->HasProperty(node->GetValue("ID")->GetStringValue())) {
				return *rvalue.GetObjectValue()->GetValue(std::to_string(i))->GetObjectValue()->GetValue(node->GetValue("ID")->GetStringValue());
			}	
		}
	}
	return Value(Undefined());
}

std::optional<Value> Evaluator::EvaluateMemberBrackets(ASTnode* node, bool insertFlag) {
	//Value expr = *Evaluate(node->GetValue("expr")->GetObjectValue(), false);
	//Value rvalue = *Evaluate(node->GetValue("lvalue")->GetObjectValue(), false);
	//if (rvalue.isUndefined()) throw RuntimeErrorException("Cannot read symbol " + expr.toString() + " of undefined");
	//if (rvalue.isObject()) {
	//	for (int i = 0; i < rvalue.GetObjectValue()->size(); i++) {

	//	}
	//}
	return Value();
}

//std::optional<Value> Evaluator::EvaluateMemberCallIdent(ASTnode* node, bool insertFlag) {
//
//}
//
//std::optional<Value> Evaluator::EvaluateMemberCallBrackets(ASTnode* node, bool insertFlag) {
//
//}

//normcall
std::optional<Value> Evaluator::EvaluateNormCall(ASTnode* node, bool insertFlag) {
	//Create a function Env

	//Evaluate the arguments and put them into an arguments table
	Value argsTable = *Evaluate(node->GetValue("argList")->GetObjectValue());

	//Invoke the target function (its value should be a function address)

	return std::nullopt;	// TODO: ??
}

//arg
std::optional<Value> Evaluator::EvaluateArg(ASTnode* node, bool insertFlag) {
	return Value();
}

//arglist
std::optional<Value> Evaluator::EvaluateArglist(ASTnode* node, bool insertFlag) {
	Object* argList = new Object();
	double numofArgs = node->GetValue("numOfArgs")->GetNumberValue();
	for (int i = 0; i < numofArgs; i++) {
		argList->Set(std::to_string(i), *Evaluate(node->GetValue(std::to_string(i))->GetObjectValue()));
	}
	return argList;
}

std::optional<Value> Evaluator::EvaluateEmptyArglist(ASTnode* node, bool insertFlag) {
	return new Object();
}

std::optional<Value> interpreter::Evaluator::EvaluateAssignExpr(ASTnode* node, bool insertFlag)
{
	Environment* savedEnvironment = EnvironmentHolder::getInstance()->GetCurrentEnv();
	Value expr =  *Evaluate(node->GetValue("expr")->GetObjectValue());
	Value& lvalue = *EvaluateLvalue(node->GetValue("lvalue")->GetObjectValue(), savedEnvironment);
	lvalue = expr;
	return lvalue;
}

// stmt
std::optional<Value> Evaluator::EvaluateIfStmt(ASTnode* node, bool insertFlag) {
	Value tmp;
	if (Evaluate(node->GetValue("condition")->GetObjectValue())->toBool()) {
		try { tmp = *Evaluate(node->GetValue("stmt")->GetObjectValue()); }
		catch (BreakException& ) { throw; }
		catch (ContinueException& ) { throw; }
		catch (ReturnException& ) { throw; }
		catch (ReturnValueException& ) { throw; }
		return true;
	}
	return false;
}

std::optional<Value> Evaluator::EvaluateIfElseStmt(ASTnode* node, bool insertFlag) {
	Value tmp;
	try {
		if (!Evaluate(node->GetValue("ifstmt")->GetObjectValue())->toBool()) {
			tmp = *Evaluate(node->GetValue("elsestmt")->GetObjectValue());
		}
		return std::nullopt;
	}
	catch (BreakException& ) { throw; }
	catch (ContinueException& ) { throw; }
	catch (ReturnException& ) { throw; }
	catch (ReturnValueException& ) { throw; }
}

std::optional<Value> Evaluator::EvaluateWhileStmt(ASTnode* node, bool insertFlag) {
	Value tmp;
	while (Evaluate(node->GetValue("condition")->GetObjectValue())->toBool()) {
		try { tmp = *Evaluate(node->GetValue("stmt")->GetObjectValue()); }
		catch (BreakException& ) { break; }
		catch (ContinueException& ) { continue; }
		catch (ReturnException& ) { throw; }
		catch (ReturnValueException& ) { throw; }
	}
	return std::nullopt;
}

std::optional<Value> Evaluator::EvaluateForStmt(ASTnode* node, bool insertFlag) {
	Value tmp;
	for (tmp = *Evaluate(node->GetValue("init_elist")->GetObjectValue());
		Evaluate(node->GetValue("condition")->GetObjectValue())->toBool();
		tmp = *Evaluate(node->GetValue("elist")->GetObjectValue())) {
		try { tmp = *Evaluate(node->GetValue("stmt")->GetObjectValue()); }
		catch (BreakException& ) { break; }
		catch (ContinueException& ) { continue; }
		catch (ReturnException& ) { throw; }
		catch (ReturnValueException& ) { throw; }
	}
	return std::nullopt;
}

std::optional<Value> Evaluator::EvaluateReturnStmt(ASTnode* node, bool insertFlag) {
	throw ReturnException();
}

std::optional<Value> Evaluator::EvaluateReturnValueStmt(ASTnode* node, bool insertFlag) {
	retVal = *Evaluate(node->GetValue("expr")->GetObjectValue());
	throw ReturnValueException();
}

std::optional<Value> Evaluator::EvaluateBreak(ASTnode* node, bool insertFlag)
{
	throw BreakException();
}

std::optional<Value> Evaluator::EvaluateContinue(ASTnode* node, bool insertFlag)
{
	throw ContinueException();
}

std::optional<Value> Evaluator::EvaluateSemicolon(ASTnode* node, bool insertFlag) { return std::nullopt; }



//elist
std::optional<Value> Evaluator::EvaluateElist(ASTnode* node, bool insertFlag) {
	Object* elistMap = new Object();
	double numOfExprs = node->GetValue("numOfExprs")->GetNumberValue();
	for (int i = 0; i < numOfExprs; i++) {
		elistMap->Set(std::to_string(i), *Evaluate(node->GetValue(std::to_string(i))->GetObjectValue()));
	}
	return elistMap;
}

std::optional<Value> Evaluator::EvaluateEmptyElist(ASTnode* node, bool insertFlag) {
	return new Object();
}

//indexed
std::optional<Value> Evaluator::EvaluateIndexed(ASTnode* node, bool insertFlag) {
	Object* indexedMap = new Object();
	double numOfElems = node->GetValue("numOfElems")->GetNumberValue();
	for (int i = 0; i < numOfElems; i++) {
		indexedMap->Set(std::to_string(i), *Evaluate(node->GetValue(std::to_string(i))->GetObjectValue()));
	}
	return indexedMap;
}

//indexedElem
std::optional<Value> Evaluator::EvaluateIndexedElem(ASTnode* node, bool insertFlag) {
	Object* newObject = new Object();
	Value key = *Evaluate(node->GetValue("keyExpr")->GetObjectValue());
	Value value = *Evaluate(node->GetValue("valueExpr")->GetObjectValue());
	newObject->Set(key, value);
	return newObject;
}


//object
std::optional<Value> Evaluator::EvaluateElistObjectdef(ASTnode* node, bool insertFlag) {
	return Evaluate(node->GetValue("elist")->GetObjectValue());
}

std::optional<Value> Evaluator::EvaluateIndexedObjectdef(ASTnode* node, bool insertFlag) {
	return Evaluate(node->GetValue("indexed")->GetObjectValue());
}


//block
std::optional<Value> Evaluator::EvaluateBlock(ASTnode* node, bool insertFlag) {
	CreateBlockEnvironment();

	Value tmp;
	double numOfStmt = node->GetValue("numOfStmt")->GetNumberValue();
	for (int i = 0; i < numOfStmt; i++) {
		tmp = *Evaluate(node->GetValue(std::to_string(i))->GetObjectValue());
	}

	LeaveBlockEnvironment();
	return std::nullopt;
}

std::optional<Value> interpreter::Evaluator::EvaluateFuncdef(ASTnode* node, bool insertFlag)
{
	Value* value = LvalueFuncDefActions(node->GetValue("ID")->GetStringValue(),node);
	if (value == nullptr) {
		throw SyntaxErrorException();
	}
	return *value;
}

std::optional<Value> interpreter::Evaluator::EvaluateAnonymousFuncdef(ASTnode* node, bool insertFlag)
{
	InsertFunctionDefinition(Object::GenerateAnonymousName(), node);
	return EnvironmentHolder::getInstance()->GetCurrentEnv();
}


// const
std::optional<Value> Evaluator::EvaluateNumberConst(ASTnode* node, bool insertFlag) {
	return node->GetValue("value")->GetNumberValue();
}

std::optional<Value> Evaluator::EvaluateStringConst(ASTnode* node, bool insertFlag) {
	return node->GetValue("value")->GetStringValue();
}

std::optional<Value> Evaluator::EvaluateBoolConst(ASTnode* node, bool insertFlag) {
	return node->GetValue("value")->GetBoolValue();
}

std::optional<Value> Evaluator::EvaluateNIL(ASTnode* node, bool insertFlag) {
	return std::nullopt;
}


//formal
std::optional<Value> Evaluator::EvaluateParam(ASTnode* node, bool insertFlag) {
	return Value();
}

std::optional<Value> Evaluator::EvaluateOptionalParam(ASTnode* node, bool insertFlag) {
	return Value();
}

//idlist
std::optional<Value> Evaluator::EvaluateIdlist(ASTnode* node, bool insertFlag) {
	// TODO: Extra error checking ensuring that all default arguments are at the end of the parameter list after any required parameters 
	Object* idList = new Object();
	double numOfParams = node->GetValue("numOfParams")->GetNumberValue();
	for (int i = 0; i < numOfParams; i++) {
		idList->Set(std::to_string(i), *Evaluate(node->GetValue(std::to_string(i))->GetObjectValue()));
	}
	return idList;
}

std::optional<Value> Evaluator::EvaluateEmptyIdlist(ASTnode* node, bool insertFlag) {
	return new Object();
}


std::optional<Value> Evaluator::EvaluatePrint(ASTnode* node, bool insertFlag) {
	//object of arguments in node ??
	for (int i = 0; i < node->size(); i++) {
		std::cout << node->GetValue(std::to_string(i));
		if (i != node->size() - 1)
			std::cout << ",";
	}
	return std::nullopt;
}

std::optional<Value> Evaluator::EvaluateTypeof(ASTnode* node, bool insertFlag) {
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

std::optional<Value> Evaluator::EvaluateObject_keys(ASTnode* node, bool insertFlag) {
	//object of arguments in node ??
	if (node->size() != 1 || !node->GetValue("0")->isObject())
		throw SyntaxErrorException("Invalid arguments in object_keys");
	Object* returnKeys = new Object();
	Object* argument = node->GetValue("0")->GetObjectValue();
	for (int i = 0; i < node->size(); i++)
	{
		returnKeys->Set(std::to_string(i), argument->GetValue(std::to_string(i))->GetObjectValue()->GetKey());
	}
	return returnKeys;
}

std::optional<Value> Evaluator::EvaluateObject_size(ASTnode* node, bool insertFlag) {
	//object of arguments in node ??
	if (node->size() != 1 || !node->GetValue("0")->isObject())
		throw SyntaxErrorException("Invalid arguments in object_size");
	return (double)node->GetValue("0")->GetObjectValue()->size();
}

std::optional<Value> Evaluator::EvaluateEval(ASTnode* node, bool insertFlag) {
	return std::nullopt;
}