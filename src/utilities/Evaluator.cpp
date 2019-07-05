#include "utilities/Evaluator.h"
#define nil (Object *)nullptr

using namespace interpreter;

std::map<std::string, std::optional<Value>(Evaluator::*)(ASTnode*)> Evaluator::IntializeDispatcher() {
	std::map<std::string, std::optional<Value>(Evaluator::*)(ASTnode*)> table;

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
	//table["member_lvalueVar"] = &Evaluator::EvaluateMemberLvalueIdent;
	//table["member_lvalueBrackets"] = &Evaluator::EvaluateMemberLvalueBrackets;
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


	return table;
}
std::map<std::string, std::optional<std::reference_wrapper<Value>>(Evaluator::*)(ASTnode*, bool, Environment*)> Evaluator::IntializeLvalueDispatcher() {
	std::map<std::string, std::optional<std::reference_wrapper<Value>>(Evaluator::*)(ASTnode*, bool, Environment*)> table;

	table["var"] = &Evaluator::EvaluateLvalueIdent;
	table["localVar"] = &Evaluator::EvaluateLvalueLocalIdent;
	table["globalVar"] = &Evaluator::EvaluateLvalueGlobalIdent;

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

std::optional<Value> Evaluator::Evaluate(ASTnode* node) {
	return (this->*EvaluateDispatcher[node->GetValue("type")->GetStringValue()])(node);
}

std::optional<std::reference_wrapper<Value>> Evaluator::EvaluateLvalue(ASTnode* node, bool insertFalg, Environment* env) {
	return (this->*EvaluateLvalueDispatcher[node->GetValue("type")->GetStringValue()])(node,insertFalg, env);
}

std::optional<Value> Evaluator::EvaluateProgram(ASTnode* node) {
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
std::optional<Value> Evaluator::EvaluateAddExpr(ASTnode* node) {
	Value left = *Evaluate(node->GetValue("left")->GetObjectValue());
	Value right = *Evaluate(node->GetValue("right")->GetObjectValue());
	return left + right;
}

std::optional<Value> Evaluator::EvaluateSubExpr(ASTnode* node) {
	Value left = *Evaluate(node->GetValue("left")->GetObjectValue());
	Value right = *Evaluate(node->GetValue("right")->GetObjectValue());
	return left - right;
}

std::optional<Value> Evaluator::EvaluateMulExpr(ASTnode* node) {
	Value left = *Evaluate(node->GetValue("left")->GetObjectValue());
	Value right = *Evaluate(node->GetValue("right")->GetObjectValue());
	return left * right;
}

std::optional<Value> Evaluator::EvaluateDivExpr(ASTnode* node) {
	Value left = *Evaluate(node->GetValue("left")->GetObjectValue());
	Value right = *Evaluate(node->GetValue("right")->GetObjectValue());
	return left / right;
}

std::optional<Value> Evaluator::EvaluateModExpr(ASTnode* node) {
	Value left = *Evaluate(node->GetValue("left")->GetObjectValue());
	Value right = *Evaluate(node->GetValue("right")->GetObjectValue());
	return left % right;
}

std::optional<Value> Evaluator::EvaluateGreaterExpr(ASTnode* node) {
	Value left = *Evaluate(node->GetValue("left")->GetObjectValue());
	Value right = *Evaluate(node->GetValue("right")->GetObjectValue());
	return left > right;
}

std::optional<Value> Evaluator::EvaluateGreaterOrEqualExpr(ASTnode* node) {
	Value left = *Evaluate(node->GetValue("left")->GetObjectValue());
	Value right = *Evaluate(node->GetValue("right")->GetObjectValue());
	return left >= right;
}

std::optional<Value> Evaluator::EvaluateLessExpr(ASTnode* node) {
	Value left = *Evaluate(node->GetValue("left")->GetObjectValue());
	Value right = *Evaluate(node->GetValue("right")->GetObjectValue());
	return left < right;
}

std::optional<Value> Evaluator::EvaluateLessOrEqualExpr(ASTnode* node) {
	Value left = *Evaluate(node->GetValue("left")->GetObjectValue());
	Value right = *Evaluate(node->GetValue("right")->GetObjectValue());
	return left <= right;
}

std::optional<Value> Evaluator::EvaluateEqualExpr(ASTnode* node) {						// TODO: na elenxoume to object 
	Value left = *Evaluate(node->GetValue("left")->GetObjectValue());
	Value right = *Evaluate(node->GetValue("right")->GetObjectValue());
	return left == right;
}

std::optional<Value> Evaluator::EvaluateNotEqualExpr(ASTnode* node) {					// TODO: na elenxoume to object 
	Value left = *Evaluate(node->GetValue("left")->GetObjectValue());
	Value right = *Evaluate(node->GetValue("right")->GetObjectValue());
	return left != right;
}

std::optional<Value> Evaluator::EvaluateAndExpr(ASTnode* node) {
	Value left = *Evaluate(node->GetValue("left")->GetObjectValue());
	Value right = *Evaluate(node->GetValue("right")->GetObjectValue());
	return left && right;
}

std::optional<Value> Evaluator::EvaluateOrExpr(ASTnode* node) {
	Value left = *Evaluate(node->GetValue("left")->GetObjectValue());
	Value right = *Evaluate(node->GetValue("right")->GetObjectValue());
	return left || right;
}


// term
std::optional<Value> Evaluator::EvaluateParenthesis(ASTnode* node) {
	return Evaluate(node->GetValue("expr")->GetObjectValue());
}

std::optional<Value> Evaluator::EvaluateUminus(ASTnode* node) {
	return (*Evaluate(node->GetValue("expr")->GetObjectValue()) * Value(-1.0));
}

std::optional<Value> Evaluator::EvaluateNot(ASTnode* node) {
	return !(*Evaluate(node->GetValue("expr")->GetObjectValue()));
}


std::optional<Value> Evaluator::EvaluatePreIncrement(ASTnode* node) {
	Value& lvalue = *EvaluateLvalue(node->GetValue("lvalue")->GetObjectValue(),false);
	return ++lvalue;
}

std::optional<Value> Evaluator::EvaluatePostIncrement(ASTnode* node) {
	Value& lvalue = *EvaluateLvalue(node->GetValue("lvalue")->GetObjectValue(), false);
	return lvalue++;
}

std::optional<Value> Evaluator::EvaluatePreDecrement(ASTnode* node) {	
	Value& lvalue = *EvaluateLvalue(node->GetValue("lvalue")->GetObjectValue(), false);
	return --lvalue;
}

std::optional<Value> Evaluator::EvaluatePostDecrement(ASTnode* node) {
	Value& lvalue = *EvaluateLvalue(node->GetValue("lvalue")->GetObjectValue(), false);
	return lvalue--;
}


// primary
std::optional<Value> Evaluator::EvaluateParenthesisFuncdef(ASTnode* node) {
	return Evaluate(node->GetValue("funcdef")->GetObjectValue()); 
}

//lvalue
std::optional<std::reference_wrapper<Value>> Evaluator::EvaluateLvalueIdent(ASTnode* node, bool insertFalg, Environment* env) {
	Value* value = LvalueVarActions(node->GetValue("ID")->GetStringValue(),insertFalg, env);
	if (value == nullptr && !insertFalg) {
		throw SyntaxErrorException();
	}
	else if (value == nullptr && insertFalg) {
		throw RuntimeErrorException();
	}
	return *value;
}

std::optional<std::reference_wrapper<Value>> Evaluator::EvaluateLvalueLocalIdent(ASTnode* node, bool insertFalg, Environment* env) {
	Value* value = LocalVarActions(node->GetValue("ID")->GetStringValue(), insertFalg, env);
	if (value == nullptr && !insertFalg) {
		throw SyntaxErrorException();
	}
	else if (value == nullptr && insertFalg) {
		throw RuntimeErrorException();
	}
	return *value;
}

std::optional<std::reference_wrapper<Value>> Evaluator::EvaluateLvalueGlobalIdent(ASTnode* node, bool insertFalg, Environment* env) {
	Value* value = GlobalVarActions(node->GetValue("ID")->GetStringValue(), env);
	if (value == nullptr) {
		throw RuntimeErrorException();
	}
	return *value;
}

//rvalue
std::optional<Value> Evaluator::EvaluateIdent(ASTnode* node) {
	return *RvalueVarActions(node->GetValue("ID")->GetStringValue());
}

std::optional<Value> Evaluator::EvaluateLocalIdent(ASTnode* node) {
	Value* value = LocalVarActions(node->GetValue("ID")->GetStringValue());
	if (value == nullptr) {
		throw SyntaxErrorException();
	}
	return *value;
}

std::optional<Value> Evaluator::EvaluateGlobalIdent(ASTnode* node) {
	Value* value = GlobalVarActions(node->GetValue("ID")->GetStringValue());
	if (value == nullptr) {
		throw RuntimeErrorException();
	}
	return *value;
}

//normcall
std::optional<Value> Evaluator::EvaluateNormCall(ASTnode* node) {
	//Create a function Env

	//Evaluate the arguments and put them into an arguments table
	Value argsTable = *Evaluate(node->GetValue("argList")->GetObjectValue());

	//Invoke the target function (its value should be a function address)

	return std::nullopt;	// TODO: ??
}

//arg
std::optional<Value> Evaluator::EvaluateArg(ASTnode* node) {
	return Value();
}

//arglist
std::optional<Value> Evaluator::EvaluateArglist(ASTnode* node) {
	Object* argList = new Object();
	double numofArgs = node->GetValue("numOfArgs")->GetNumberValue();
	for (int i = 0; i < numofArgs; i++) {
		argList->Set(std::to_string(i), *Evaluate(node->GetValue(std::to_string(i))->GetObjectValue()));
	}
	return argList;
}

std::optional<Value> Evaluator::EvaluateEmptyArglist(ASTnode* node) {
	return new Object();
}

std::optional<Value> interpreter::Evaluator::EvaluateAssignExpr(ASTnode* node)
{
	Environment* savedEnvironment = EnvironmentHolder::getInstance()->GetCurrentEnv();
	Value expr =  *Evaluate(node->GetValue("expr")->GetObjectValue());
	Value& lvalue = *EvaluateLvalue(node->GetValue("lvalue")->GetObjectValue(), savedEnvironment);
	lvalue = expr;
	return lvalue;
}

// stmt
std::optional<Value> Evaluator::EvaluateIfStmt(ASTnode* node) {
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

std::optional<Value> Evaluator::EvaluateIfElseStmt(ASTnode* node) {
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

std::optional<Value> Evaluator::EvaluateWhileStmt(ASTnode* node) {
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

std::optional<Value> Evaluator::EvaluateForStmt(ASTnode* node) {
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

std::optional<Value> Evaluator::EvaluateReturnStmt(ASTnode* node) {
	throw ReturnException();
}

std::optional<Value> Evaluator::EvaluateReturnValueStmt(ASTnode* node) {
	retVal = *Evaluate(node->GetValue("expr")->GetObjectValue());
	throw ReturnValueException();
}

std::optional<Value> Evaluator::EvaluateBreak(ASTnode* node)
{
	throw BreakException();
}

std::optional<Value> Evaluator::EvaluateContinue(ASTnode* node)
{
	throw ContinueException();
}

std::optional<Value> Evaluator::EvaluateSemicolon(ASTnode* node) { return std::nullopt; }



//elist
std::optional<Value> Evaluator::EvaluateElist(ASTnode* node) {
	Object* elistMap = new Object();
	double numOfExprs = node->GetValue("numOfExprs")->GetNumberValue();
	for (int i = 0; i < numOfExprs; i++) {
		elistMap->Set(std::to_string(i), *Evaluate(node->GetValue(std::to_string(i))->GetObjectValue()));
	}
	return elistMap;
}

std::optional<Value> Evaluator::EvaluateEmptyElist(ASTnode* node) {
	return new Object();
}

//indexed
std::optional<Value> Evaluator::EvaluateIndexed(ASTnode* node) {
	Object* indexedMap = new Object();
	double numOfElems = node->GetValue("numOfElems")->GetNumberValue();
	for (int i = 0; i < numOfElems; i++) {
		indexedMap->Set(std::to_string(i), *Evaluate(node->GetValue(std::to_string(i))->GetObjectValue()));
	}
	return indexedMap;
}

//indexedElem
std::optional<Value> Evaluator::EvaluateIndexedElem(ASTnode* node) {
	Object* newObject = new Object();
	Value key = *Evaluate(node->GetValue("keyExpr")->GetObjectValue());
	Value value = *Evaluate(node->GetValue("valueExpr")->GetObjectValue());
	newObject->Set(key, value);
	return newObject;
}


//object
std::optional<Value> Evaluator::EvaluateElistObjectdef(ASTnode* node) {
	return Evaluate(node->GetValue("elist")->GetObjectValue());
}

std::optional<Value> Evaluator::EvaluateIndexedObjectdef(ASTnode* node) {
	return Evaluate(node->GetValue("indexed")->GetObjectValue());
}


//block
std::optional<Value> Evaluator::EvaluateBlock(ASTnode* node) {
	CreateBlockEnvironment();

	Value tmp;
	double numOfStmt = node->GetValue("numOfStmt")->GetNumberValue();
	for (int i = 0; i < numOfStmt; i++) {
		tmp = *Evaluate(node->GetValue(std::to_string(i))->GetObjectValue());
	}

	LeaveBlockEnvironment();
	return std::nullopt;
}

std::optional<Value> interpreter::Evaluator::EvaluateFuncdef(ASTnode* node)
{
	Value* value = LvalueFuncDefActions(node->GetValue("ID")->GetStringValue(),node);
	if (value == nullptr) {
		throw SyntaxErrorException();
	}
	return *value;
}

std::optional<Value> interpreter::Evaluator::EvaluateAnonymousFuncdef(ASTnode* node)
{
	InsertFunctionDefinition(Object::GenerateAnonymousName(), node);
	return EnvironmentHolder::getInstance()->GetCurrentEnv();
}


// const
std::optional<Value> Evaluator::EvaluateNumberConst(ASTnode* node) {
	return node->GetValue("value")->GetNumberValue();
}

std::optional<Value> Evaluator::EvaluateStringConst(ASTnode* node) {
	return node->GetValue("value")->GetStringValue();
}

std::optional<Value> Evaluator::EvaluateBoolConst(ASTnode* node) {
	return node->GetValue("value")->GetBoolValue();
}

std::optional<Value> Evaluator::EvaluateNIL(ASTnode* node) {
	return std::nullopt;
}


//formal
std::optional<Value> Evaluator::EvaluateParam(ASTnode* node) {
	return Value();
}

std::optional<Value> Evaluator::EvaluateOptionalParam(ASTnode* node) {
	return Value();
}

//idlist
std::optional<Value> Evaluator::EvaluateIdlist(ASTnode* node) {
	// TODO: Extra error checking ensuring that all default arguments are at the end of the parameter list after any required parameters 
	Object* idList = new Object();
	double numOfParams = node->GetValue("numOfParams")->GetNumberValue();
	for (int i = 0; i < numOfParams; i++) {
		idList->Set(std::to_string(i), *Evaluate(node->GetValue(std::to_string(i))->GetObjectValue()));
	}
	return idList;
}

std::optional<Value> Evaluator::EvaluateEmptyIdlist(ASTnode* node) {
	return new Object();
}
