#include "utilities/Evaluator.h"
#define nil (Object *)nullptr

using namespace interpreter;

std::map<std::string, Value(Evaluator::*)(ASTnode*)> Evaluator::IntializeDispatcher() {
	std::map<std::string, Value(Evaluator::*)(ASTnode*)> table;

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
	//table["pre_increment"] = &Evaluator::EvaluatePreIncrement;
	//table["post_incremen"] = &Evaluator::EvaluatePostIncrement;
	//table["pre_decrement"] = &Evaluator::EvaluatePreDecrement;
	//table["post_decrement"] = &Evaluator::EvaluatePostDecrement;
	table["parentheses_funcdef"] = &Evaluator::EvaluateParenthesisFuncdef;
	//table["lvalueVar"] = &Evaluator::EvaluateIdent;
	//table["localVar"] = &Evaluator::EvaluateLocalIdent;
	//table["globalVar"] = &Evaluator::EvaluateGlobalIdent;
	//table["member_lvalueVar"] = &Evaluator::EvaluateLvalueIdent;
	//table["member_lvalueBrackets"] = &Evaluator::EvaluateLvalueBrackets;
	//table["member_callVar"] = &Evaluator::EvaluateCallIdent;
	//table["member_callBrackets"] = &Evaluator::EvaluateCallBrackets;
	//table["multiCall"] = &Evaluator::EvaluateMultiCall;
	//table["lvalueCall"] = &Evaluator::EvaluateLvalueCallSuffix;
	//table["funcdefCall"] = &Evaluator::EvaluateFuncdefCall;
	table["normcall"] = &Evaluator::EvaluateNormCall;
	//table["methodcall"] = &Evaluator::EvaluateMethodCall;
	//table["namedParam"] = &Evaluator::EvaluateArg;
	table["argList"] = &Evaluator::EvaluateArglist;
	table["emptyArgList"] = &Evaluator::EvaluateEmptyArglist;
	//table["assignexpr"] = &Evaluator::EvaluateAssignExpr;
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


Evaluator::Evaluator() {
	EvaluateDispatcher = IntializeDispatcher();
}

Evaluator* Evaluator::getInstance() {
	if (!evaluator)
		evaluator = new Evaluator();
	return evaluator;
}

Value Evaluator::Evaluate(ASTnode* node) {
	return (this->*EvaluateDispatcher[node->GetValue("type").GetStringValue()])(node);
}

Value Evaluator::EvaluateProgram(ASTnode* node) {
	InitGlobalEnvironment();

	Value tmp;
	double numOfStmt = node->GetValue("numOfStmt").GetNumberValue();
	for (int i = 0; i < numOfStmt; i++) {
		try { tmp = Evaluate(node->GetValue(std::to_string(i)).GetObjectValue()); }
		catch (const std::exception& e) { 
			std::cout << std::endl << e.what() << std::endl;  
			exit(0); 
		}
	}
	return nil;
}

// expr
Value Evaluator::EvaluateAddExpr(ASTnode* node) {
	Value left = Evaluate(node->GetValue("left").GetObjectValue());
	Value right = Evaluate(node->GetValue("right").GetObjectValue());
	return left + right;
}

Value Evaluator::EvaluateSubExpr(ASTnode* node) {
	Value left = Evaluate(node->GetValue("left").GetObjectValue());
	Value right = Evaluate(node->GetValue("right").GetObjectValue());
	return left - right;
}

Value Evaluator::EvaluateMulExpr(ASTnode* node) {
	Value left = Evaluate(node->GetValue("left").GetObjectValue());
	Value right = Evaluate(node->GetValue("right").GetObjectValue());
	return left * right;
}

Value Evaluator::EvaluateDivExpr(ASTnode* node) {
	Value left = Evaluate(node->GetValue("left").GetObjectValue());
	Value right = Evaluate(node->GetValue("right").GetObjectValue());
	return left / right;
}

Value Evaluator::EvaluateModExpr(ASTnode* node) {
	Value left = Evaluate(node->GetValue("left").GetObjectValue());
	Value right = Evaluate(node->GetValue("right").GetObjectValue());
	return left % right;
}

Value Evaluator::EvaluateGreaterExpr(ASTnode* node) {
	Value left = Evaluate(node->GetValue("left").GetObjectValue());
	Value right = Evaluate(node->GetValue("right").GetObjectValue());
	return left > right;
}

Value Evaluator::EvaluateGreaterOrEqualExpr(ASTnode* node) {
	Value left = Evaluate(node->GetValue("left").GetObjectValue());
	Value right = Evaluate(node->GetValue("right").GetObjectValue());
	return left >= right;
}

Value Evaluator::EvaluateLessExpr(ASTnode* node) {
	Value left = Evaluate(node->GetValue("left").GetObjectValue());
	Value right = Evaluate(node->GetValue("right").GetObjectValue());
	return left < right;
}

Value Evaluator::EvaluateLessOrEqualExpr(ASTnode* node) {
	Value left = Evaluate(node->GetValue("left").GetObjectValue());
	Value right = Evaluate(node->GetValue("right").GetObjectValue());
	return left <= right;
}

Value Evaluator::EvaluateEqualExpr(ASTnode* node) {						// TODO: na elenxoume to object 
	Value left = Evaluate(node->GetValue("left").GetObjectValue());
	Value right = Evaluate(node->GetValue("right").GetObjectValue());
	return left == right;
}

Value Evaluator::EvaluateNotEqualExpr(ASTnode* node) {					// TODO: na elenxoume to object 
	Value left = Evaluate(node->GetValue("left").GetObjectValue());
	Value right = Evaluate(node->GetValue("right").GetObjectValue());
	return left != right;
}

Value Evaluator::EvaluateAndExpr(ASTnode* node) {
	Value left = Evaluate(node->GetValue("left").GetObjectValue());
	Value right = Evaluate(node->GetValue("right").GetObjectValue());
	return left && right;
}

Value Evaluator::EvaluateOrExpr(ASTnode* node) {
	Value left = Evaluate(node->GetValue("left").GetObjectValue());
	Value right = Evaluate(node->GetValue("right").GetObjectValue());
	return left || right;
}


// term
Value Evaluator::EvaluateParenthesis(ASTnode* node) {
	return Evaluate(node->GetValue("expr").GetObjectValue());
}

Value Evaluator::EvaluateUminus(ASTnode* node) {
	return Evaluate(node->GetValue("expr").GetObjectValue()) * Value(-1.0);
}

Value Evaluator::EvaluateNot(ASTnode* node) {
	return !Evaluate(node->GetValue("expr").GetObjectValue());
}

/*
Value Evaluator::EvaluatePreIncrement(ASTnode* node) {				// TODO
	ASTnode* lvalueNode = node->GetValue("lvalue").GetObjectValue();
	
	ASTnode * num = new ASTnode("type", "numConst");
	num->Set("value", 1.0);
	ASTnode* add = new ASTnode("type", "ADD");
	add->Set("left", lvalueNode);
	add->Set("right", num);
	ASTnode* assign = new ASTnode("type", "assignexpr");
	assign->Set("lvalue", lvalueNode);
	assign->Set("expr", add);

	return Evaluate(add);
}

Value Evaluator::EvaluatePostIncrement(ASTnode* node) {
	Value old = Evaluate(node->GetValue("lvalue").GetObjectValue());
	
	// TODO: na allaksei kai na kanoume oti kanei h assign sto environment kai allazei thn timh to komvoy kai oxi ASTnodes
	ASTnode* num = new ASTnode("type", "numConst");
	num->Set("value", 1.0);
	ASTnode* add = new ASTnode("type", "ADD"); 
	add->Set("left", old);
	add->Set("right", num);
	ASTnode* assign = new ASTnode("type", "assignexpr");
	assign->Set("lvalue", old);
	assign->Set("expr", add);
	Evaluate(add);
	
	return old;
}

Value Evaluator::EvaluatePreDecrement(ASTnode* node) {				// TODO
	ASTnode* lvalueNode = node->GetValue("lvalue").GetObjectValue();
	ASTnode* num = new ASTnode("type", "numConst");
	num->Set("value", 1.0);
	ASTnode* sub = new ASTnode("type", "SUB");
	sub->Set("left", lvalueNode);
	sub->Set("right", num);
	ASTnode* assign = new ASTnode("type", "assignexpr");
	assign->Set("lvalue", lvalueNode);
	assign->Set("expr", sub);

	return Evaluate(sub);
}

Value Evaluator::EvaluatePostDecrement(ASTnode* node) {
	Value old = Evaluate(node->GetValue("lvalue").GetObjectValue());

	// TODO: na allaksei kai na kanoume oti kanei h assign sto environment kai allazei thn timh to komvoy kai oxi ASTnodes
	ASTnode* num = new ASTnode("type", "numConst");
	num->Set("value", 1.0);
	ASTnode* sub = new ASTnode("type", "SUB");
	sub->Set("left", old);
	sub->Set("right", num);
	ASTnode* assign = new ASTnode("type", "assignexpr");
	assign->Set("lvalue", old);
	assign->Set("expr", sub);
	Evaluate(sub);

	return old;
}
*/

// primary
Value Evaluator::EvaluateParenthesisFuncdef(ASTnode* node) {
	return Evaluate(node->GetValue("funcdef").GetObjectValue()); 
}

//lvalue
Value Evaluator::EvaluateIdent(ASTnode* node) {
	//return the environment that includes id
	//Value block = EnvironmentHolder::getInstance()->LookupNormal(node->GetValue("ID").GetStringValue());
	//if(var)
		//check for access
		//return block;
	//else {
		//EnvironmentHolder::getInstance()->GetCurrentEnv()->Set(node->GetValue("ID").GetStringValue(),Value());
		//return EnvironmentHolder::getInstance()->GetCurrentEnv();
	//}
		
	return Value();
}

Value Evaluator::EvaluateLocalIdent(ASTnode* node) {
	//Value block = EnvironmentHolder::getInstance()->LookupLocal(node->GetValue("ID").GetStringValue());
	//if( block )
		//return block;
	//else if( !var && (!CollisionLibFunc() || GlobalScope() ) ){
		//EnvironmentHolder::getInstance()->GetCurrentEnv()->Set(node->GetValue("ID").GetStringValue(),Value());
		//return EnvironmentHolder::getInstance()->GetCurrentEnv();
	//}
	//else
		//error collision with lib func
	return Value();
}

Value Evaluator::EvaluateGlobalIdent(ASTnode* node) {
	//Value block = EnvironmentHolder::getInstance()->LookupGlobal(node->GetValue("ID").GetStringValue());
	//if (block)
		//return block;
	//else
		//error no symbol found (no insertion)
	return Value();
}

//normcall
Value Evaluator::EvaluateNormCall(ASTnode* node) {
	//Create a function Env

	//Evaluate the arguments and put them into an arguments table
	Value argsTable = Evaluate(node->GetValue("argList").GetObjectValue());

	//Invoke the target function (its value should be a function address)

	return nil;	// TODO: ??
}

//arg
Value Evaluator::EvaluateArg(ASTnode* node) {
	return Value();
}

//arglist
Value Evaluator::EvaluateArglist(ASTnode* node) {
	Object* argList = new Object();
	double numofArgs = node->GetValue("numOfArgs").GetNumberValue();
	for (int i = 0; i < numofArgs; i++) {
		argList->Set(std::to_string(i), Evaluate(node->GetValue(std::to_string(i)).GetObjectValue()));
	}
	return argList;
}

Value Evaluator::EvaluateEmptyArglist(ASTnode* node) {
	return new Object();
}

// stmt
Value Evaluator::EvaluateIfStmt(ASTnode* node) {
	Value tmp;
	if (Evaluate(node->GetValue("condition").GetObjectValue()).toBool()) {
		try { tmp = Evaluate(node->GetValue("stmt").GetObjectValue()); }
		catch (BreakException& ) { throw; }
		catch (ContinueException& ) { throw; }
		catch (ReturnException& ) { throw; }
		catch (ReturnValueException& ) { throw; }
		return true;
	}
	return false;
}

Value Evaluator::EvaluateIfElseStmt(ASTnode* node) {
	Value tmp;
	try {
		if (!Evaluate(node->GetValue("ifstmt").GetObjectValue()).toBool()) {
			tmp = Evaluate(node->GetValue("elsestmt").GetObjectValue());
		}
		return nil;
	}
	catch (BreakException& ) { throw; }
	catch (ContinueException& ) { throw; }
	catch (ReturnException& ) { throw; }
	catch (ReturnValueException& ) { throw; }
}

Value Evaluator::EvaluateWhileStmt(ASTnode* node) {
	Value tmp;
	while (Evaluate(node->GetValue("condition").GetObjectValue()).toBool()) {
		try { tmp = Evaluate(node->GetValue("stmt").GetObjectValue()); }
		catch (BreakException& ) { break; }
		catch (ContinueException& ) { continue; }
		catch (ReturnException& ) { throw; }
		catch (ReturnValueException& ) { throw; }
	}
	return nil;
}

Value Evaluator::EvaluateForStmt(ASTnode* node) {
	Value tmp;
	for (tmp = Evaluate(node->GetValue("init_elist").GetObjectValue());
		Evaluate(node->GetValue("condition").GetObjectValue()).toBool();
		tmp = Evaluate(node->GetValue("elist").GetObjectValue())) {
		try { tmp = Evaluate(node->GetValue("stmt").GetObjectValue()); }
		catch (BreakException& ) { break; }
		catch (ContinueException& ) { continue; }
		catch (ReturnException& ) { throw; }
		catch (ReturnValueException& ) { throw; }
	}
	return nil;
}

Value Evaluator::EvaluateReturnStmt(ASTnode* node) {
	throw ReturnException();
}

Value Evaluator::EvaluateReturnValueStmt(ASTnode* node) {
	retVal = Evaluate(node->GetValue("expr").GetObjectValue());
	throw ReturnValueException();
}

Value Evaluator::EvaluateBreak(ASTnode* node)
{
	throw BreakException();
}

Value Evaluator::EvaluateContinue(ASTnode* node)
{
	throw ContinueException();
}

Value Evaluator::EvaluateSemicolon(ASTnode* node) { return nil; };



//elist
Value Evaluator::EvaluateElist(ASTnode* node) {
	Object* elistMap = new Object();
	double numOfExprs = node->GetValue("numOfExprs").GetNumberValue();
	for (int i = 0; i < numOfExprs; i++) {
		elistMap->Set(std::to_string(i), Evaluate(node->GetValue(std::to_string(i)).GetObjectValue()));
	}
	return elistMap;
}

Value Evaluator::EvaluateEmptyElist(ASTnode* node) {
	return new Object();
}

//indexed
Value Evaluator::EvaluateIndexed(ASTnode* node) {
	Object* indexedMap = new Object();
	double numOfElems = node->GetValue("numOfElems").GetNumberValue();
	for (int i = 0; i < numOfElems; i++) {
		indexedMap->Set(std::to_string(i), Evaluate(node->GetValue(std::to_string(i)).GetObjectValue()));
	}
	return indexedMap;
}

//indexedElem
Value Evaluator::EvaluateIndexedElem(ASTnode* node) {
	Object* newObject = new Object();
	Value key = Evaluate(node->GetValue("keyExpr").GetObjectValue());
	Value value = Evaluate(node->GetValue("valueExpr").GetObjectValue());
	newObject->Set(key, value);
	return newObject;
}


//object
Value Evaluator::EvaluateElistObjectdef(ASTnode* node) {
	return Evaluate(node->GetValue("elist").GetObjectValue());
}

Value Evaluator::EvaluateIndexedObjectdef(ASTnode* node) {
	return Evaluate(node->GetValue("indexed").GetObjectValue());
}


//block
Value Evaluator::EvaluateBlock(ASTnode* node) {
	CreateBlockEnvironment();

	Value tmp;
	double numOfStmt = node->GetValue("numOfStmt").GetNumberValue();
	for (int i = 0; i < numOfStmt; i++) {
		tmp = Evaluate(node->GetValue(std::to_string(i)).GetObjectValue());
	}

	LeaveBlockEnvironment();
	return nil;
}

Value interpreter::Evaluator::EvaluateFuncdef(ASTnode* node)
{
	//Value var = EnvironmentHolder::getInstance()->LookupLocal(node->GetValue("ID").GetStringValue());
	//if(var || CollisionLibFunc())
		//error collision or symbol is found
	InsertFunctionDefinition(node->GetValue("ID").GetStringValue(),node);
	
	return nil;
}

Value interpreter::Evaluator::EvaluateAnonymousFuncdef(ASTnode* node)
{
	InsertFunctionDefinition(Object::GenerateAnonymousName(), node);

	return nil;
}


// const
Value Evaluator::EvaluateNumberConst(ASTnode* node) {
	return node->GetValue("value").GetNumberValue();
}

Value Evaluator::EvaluateStringConst(ASTnode* node) {
	return node->GetValue("value").GetStringValue();
}

Value Evaluator::EvaluateBoolConst(ASTnode* node) {
	return node->GetValue("value").GetBoolValue();
}

Value Evaluator::EvaluateNIL(ASTnode* node) {
	return nil;
}


//formal
Value Evaluator::EvaluateParam(ASTnode* node) {
	return Value();
}

Value Evaluator::EvaluateOptionalParam(ASTnode* node) {
	return Value();
}

//idlist
Value Evaluator::EvaluateIdlist(ASTnode* node) {
	// TODO: Extra error checking ensuring that all default arguments are at the end of the parameter list after any required parameters 
	Object* idList = new Object();
	double numOfParams = node->GetValue("numOfParams").GetNumberValue();
	for (int i = 0; i < numOfParams; i++) {
		idList->Set(std::to_string(i), Evaluate(node->GetValue(std::to_string(i)).GetObjectValue()));
	}
	return idList;
}

Value Evaluator::EvaluateEmptyIdlist(ASTnode* node) {
	return new Object();
}
