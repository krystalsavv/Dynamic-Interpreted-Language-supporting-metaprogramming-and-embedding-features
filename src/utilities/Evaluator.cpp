#include "utilities/Evaluator.h"
#define nil (Object *)nullptr

std::map<std::string, Value(Evaluator::*)(ASTnode*)> Evaluator::IntializeDispatcher() {
	std::map<std::string, Value(Evaluator::*)(ASTnode*)> table;
	// term
	table["parentheses"] = &Evaluator::EvaluateParenthesis; 
	table["uminus"] = &Evaluator::EvaluateUminus;
	table["not"] = &Evaluator::EvaluateNot;
	//table["PRE_INCREMENT"] = &Evaluator::EvaluatePreIncrement;
	//table["POST_INCREMENT"] = &Evaluator::EvaluatePostIncrement;
	//table["PRE_DECREMENT"] = &Evaluator::EvaluatePreDecrement;
	//table["POST_DECREMENT"] = &Evaluator::EvaluatePostDecrement;
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
	table["semicolon"] = &Evaluator::EvaluateSemicolon;
	/*table["return"] = &Evaluator::EvaluateReturnStmt;
	table["return_value"] = &Evaluator::EvaluateReturnValueStmt;*/
	
	return table;
}

std::map<std::string, Value(Evaluator::*)(ASTnode*) > Evaluator::EvaluateDispatcher = IntializeDispatcher();


Value Evaluator::Evaluate(ASTnode* node) {
	return EvaluateDispatcher[node->GetValue("type").GetStringValue()];
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
	if (Evaluate(node->GetValue("condition").GetObjectValue()).toBool()) {
		auto tmp = Evaluate(node->GetValue("stmt").GetObjectValue());
		return true;
	}
	return false;
}

Value Evaluator::EvaluateIfElseStmt(ASTnode* node) {
	if (!Evaluate(node->GetValue("ifstmt").GetObjectValue()).toBool()) {
		auto tmp = Evaluate(node->GetValue("elsestmt").GetObjectValue());
	}
	return nil;
}

Value Evaluator::EvaluateWhileStmt(ASTnode* node) {
	while (Evaluate(node->GetValue("condition").GetObjectValue()).toBool()) {
		auto tmp = Evaluate(node->GetValue("stmt").GetObjectValue());
	}
	return nil;
}

Value Evaluator::EvaluateForStmt(ASTnode* node) {
	Value tmp;
	for ( tmp = Evaluate(node->GetValue("init_elist").GetObjectValue()); 
		  Evaluate(node->GetValue("condition").GetObjectValue()).toBool(); 
		  tmp = Evaluate(node->GetValue("elist").GetObjectValue()) ) {
		tmp = Evaluate(node->GetValue("stmt").GetObjectValue());
	}
	return nil;
}

//Value Evaluator::EvaluateReturnStmt(ASTnode* node) {
//
//}

Value Evaluator::EvaluateSemicolon(ASTnode* node) { return nil; };
