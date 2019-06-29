#include "utilities/Evaluator.h"
#define nil (Object *)nullptr

using namespace interpreter;

std::map<std::string, Value(Evaluator::*)(ASTnode*)> Evaluator::IntializeDispatcher() {
	std::map<std::string, Value(Evaluator::*)(ASTnode*)> table;
	table["program"] = &Evaluator::EvaluateProgram;
	
	// term
	table["parentheses"] = &Evaluator::EvaluateParenthesis; 
	table["uminus"] = &Evaluator::EvaluateUminus;
	table["not"] = &Evaluator::EvaluateNot;
	//table["pre_increment"] = &Evaluator::EvaluatePreIncrement;
	//table["post_incremen"] = &Evaluator::EvaluatePostIncrement;
	//table["pre_decrement"] = &Evaluator::EvaluatePreDecrement;
	//table["post_decrement"] = &Evaluator::EvaluatePostDecrement;
	//const 
	table["numConst"] = &Evaluator::EvaluateNumberConst;
	table["stringConst"] = &Evaluator::EvaluateStringConst;
	table["boolConst"] = &Evaluator::EvaluateBoolConst;
	table["nil"] = &Evaluator::EvaluateNIL;
	// expr
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

	//stmt
	table["ifstmt"] = &Evaluator::EvaluateIfStmt;
	table["if_elsestmt"] = &Evaluator::EvaluateIfElseStmt;
	table["whilestmt"] = &Evaluator::EvaluateWhileStmt;
	table["forstmt"] = &Evaluator::EvaluateForStmt;
	table["return"] = &Evaluator::EvaluateReturnStmt;
	table["return_value"] = &Evaluator::EvaluateReturnValueStmt;
	table["break"] = &Evaluator::EvaluateBreak;
	table["continue"] = &Evaluator::EvaluateContinue;
	table["block"] = &Evaluator::EvaluateBlock;
	table["semicolon"] = &Evaluator::EvaluateSemicolon;

	//elist
	table["elist"] = &Evaluator::EvaluateElist;
	table["emptyElist"] = &Evaluator::EvaluateEmptyElist;
	
	//indexed
	table["indexed"] = &Evaluator::EvaluateIndexed;

	//indexedElem
	table["indexedelem"] = &Evaluator::EvaluateIndexedElem;

	//object
	table["elistObjectdef"] = &Evaluator::EvaluateElistObjectdef;
	//table["indexedObjectdef"] = &Evaluator::EvaluateIndexedObjectdef;

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
		catch (const std::exception& e) { std::cout << std::endl << e.what() << std::endl;  exit(0); }
	}
	return nil;
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


// stmt
Value Evaluator::EvaluateIfStmt(ASTnode* node){
	Value tmp;
	if (Evaluate(node->GetValue("condition").GetObjectValue()).toBool()) {
		try { tmp = Evaluate(node->GetValue("stmt").GetObjectValue()); }
		catch (BreakException& e) { throw; }
		catch (ContinueException& e) { throw; }
		catch (ReturnException& e) { throw; }
		catch (ReturnValueException& e) { throw; }
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
	catch (BreakException& e) { throw; }
	catch (ContinueException& e) { throw; }
	catch (ReturnException& e) { throw; }
	catch (ReturnValueException& e) { throw; }
}

Value Evaluator::EvaluateWhileStmt(ASTnode* node) {
	Value tmp;
	while (Evaluate(node->GetValue("condition").GetObjectValue()).toBool()) {
		try { tmp = Evaluate(node->GetValue("stmt").GetObjectValue()); }
		catch (BreakException& e) { break; }
		catch (ContinueException& e) { continue; }
		catch (ReturnException& e) { throw; }
		catch (ReturnValueException& e) { throw; }
	}
	return nil;
}

Value Evaluator::EvaluateForStmt(ASTnode* node) {
	Value tmp;
	for ( tmp = Evaluate(node->GetValue("init_elist").GetObjectValue()); 
		  Evaluate(node->GetValue("condition").GetObjectValue()).toBool(); 
		  tmp = Evaluate(node->GetValue("elist").GetObjectValue()) ) {
		try { tmp = Evaluate(node->GetValue("stmt").GetObjectValue()); }
		catch (BreakException& e) { break; }
		catch (ContinueException& e) { continue; }
		catch (ReturnException& e) { throw; }
		catch (ReturnValueException& e) { throw; }
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


//elist
Value Evaluator::EvaluateElist(ASTnode* node) {
	Object* elistMap = new Object(); 
	double numOfExpr = node->GetValue("numOfExpr").GetNumberValue();
	for (int i = 0; i < numOfExpr; i++) {
		 elistMap->Set(std::to_string(i), Evaluate(node->GetValue(std::to_string(i)).GetObjectValue()));
	}
	return elistMap;
}

Value Evaluator::EvaluateEmptyElist(ASTnode* node) {
	Object* emptyElistMap = new Object();
	return emptyElistMap;
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
	newObject->Set(key,value);
	return newObject;
}


//object
Value Evaluator::EvaluateElistObjectdef(ASTnode* node) {
	return Evaluate(node->GetValue("elist").GetObjectValue());
}

Value Evaluator::EvaluateIndexedObjectdef(ASTnode* node) {
	return Evaluate(node->GetValue("indexed").GetObjectValue());
}

