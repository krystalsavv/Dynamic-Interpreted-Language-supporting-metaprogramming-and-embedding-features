#include "utilities/EvaluatorActions.h"

using namespace interpreter;

Value* interpreter::LvalueVarActions(std::string id, bool insertFlag, Environment* envIterator)
{
	Value* value = NormalLookUp(id, envIterator);
	if (value != nullptr && !hasCollisionWithLibFunc(id))
		return value;
	else if (value == nullptr && insertFlag)
		return InsertLvalue(id, Value(), envIterator);
	else
		return nullptr;
}

Value* interpreter::LvalueLocalVarActions(std::string id, bool insertFlag, Environment* envIterator) {
	Value* value = LocalLookUp(id, envIterator);
	if (hasCollisionWithLibFunc(id)) {
		return nullptr;
	}
	else if (value != nullptr) {
		return value;
	}
	else {
		if (insertFlag) {
			return InsertLvalue(id, Value(), envIterator);
		}
		else {
			return nullptr;
		}
	}
}

Value* interpreter::GlobalVarActions(std::string id, Environment* envIterator) {
	return GlobalLookUp(id, envIterator);
}

Value* interpreter::LvalueFuncDefActions(std::string id, ASTnode* node) {
	Value* value = LocalLookUp(id);
	if (value != nullptr || hasCollisionWithLibFunc(id)) {
		return nullptr;
	}
	return InsertFunctionDefinition(id, node);
}

Value* interpreter::RvalueVarActions(std::string id, bool insertFlag)
{
	Value* value = NormalLookUp(id);
	if (value != nullptr)
		return value;
	else if (insertFlag)
		return InsertLvalue(id, Value());
	else
		return nullptr;
}

Value* interpreter::RvalueLocalVarActions(std::string id, bool insertFlag)
{
	Value* value = LocalLookUp(id);
	if (hasCollisionWithLibFunc(id) && !EnvironmentHolder::getInstance()->isGlobalScope()) {
		return nullptr;
	}
	else if (value != nullptr) {
		return value;
	}
	else if (insertFlag) {
		return InsertLvalue(id, Value());
	}
	return nullptr;
}

Value& interpreter::CallerEnvironmentActions(Value& funcdefNode, bool isFunctor) {
	/*
	already checked at the lvalue evaluation
	if (funcdefNode == nullptr) {
		throw RuntimeErrorException("Can not find function definition with name " );
	}*/
	if (!(funcdefNode.isObject())) {
		throw RuntimeErrorException("Variable with name " + funcdefNode.GetObjectValue()->GetValue("ID")->GetStringValue() + "is not callable");
	}
	else {
		Value* closure = funcdefNode.GetObjectValue()->GetValue("$closure");
		if (closure != nullptr) {
			if (isFunctor)
				//Add GetObjectValue()->GetValue("ID")->GetStringValue() as first arg
				CreateFunctionEnvironment(closure->GetObjectValue());
			return funcdefNode;
		}
		else {
			if (!funcdefNode.GetObjectValue()->HasProperty("()"))
				throw RuntimeErrorException("Object with name " + funcdefNode.GetObjectValue()->GetValue("ID")->GetStringValue() + "is not a functor");
			return CallerEnvironmentActions(*funcdefNode.GetObjectValue()->GetValue("()"), true);
		}
	}
}

void interpreter::AddPositionalParamsToEnvironment(Object* idList, Object* argTable) {
	Object* PositionalArgs = argTable->GetValue("PositionalArgs")->GetObjectValue();
	double numOfPositionalArgs = argTable->GetValue("numOfPositionalArgs")->GetNumberValue();
	Environment* curr = EnvironmentHolder::getInstance()->GetCurrentEnv();
	for (double i = 0; i < numOfPositionalArgs; ++i) {
		curr->Set(*(idList->GetValue(i)->GetObjectValue()->GetValue("ID")), *(PositionalArgs->GetValue(i)));
	}
}

void interpreter::AddNamedParamsToEnvironment(Object& idList_withoutIndex, Object* argTable) {
	Object* NamedArgs = argTable->GetValue("NamedArgs")->GetObjectValue();
	Environment* curr = EnvironmentHolder::getInstance()->GetCurrentEnv();
	for (auto kv : NamedArgs->GetMap()) {
		std::string id = kv.first.GetStringValue();
		if (LocalLookUp(id)) throw RuntimeErrorException("Paramiter " + id + "both positional and named value");
		if (!(idList_withoutIndex.HasProperty(id))) throw RuntimeErrorException("Unexpected named parameter " + id);
		curr->Set(id, kv.second);
	}
}

Object* interpreter::AddLvalueAsFirstArg(Value& lvalue) {
	double numOfPositionalArgs = argTable->GetValue("numOfPositionalArgs")->GetNumberValue();
	double numOfTotalArgs = argTable->GetValue("numOfTotalArgs")->GetNumberValue();
	Object* PositionalArgs = argTable->GetValue("PositionalArgs")->GetObjectValue();
	Object* newArgTable = new Object(*argTable);
	if (PositionalArgs == nullptr)
		newArgTable->Set("PositionalArgs", new Object());

	for (double i = 0; i < numOfPositionalArgs; ++i) {
		newArgTable->GetValue("PositionalArgs")->GetObjectValue()->Set(i + 1, *(PositionalArgs->GetValue(i)));
	}
	newArgTable->Set("numOfPositionalArgs", numOfPositionalArgs + 1);
	newArgTable->Set("numOfTotalArgs", numOfTotalArgs + 1);
	newArgTable->GetValue("PositionalArgs")->GetObjectValue()->Set(0.0, lvalue);
	return newArgTable;
}

Value& interpreter::Object_set(Value& lvalue, std::string id) {
	if (lvalue.GetObjectValue()->HasProperty("$closure")) {
		Value* closureVariable = LocalLookUp(id, lvalue.GetObjectValue()->GetValue("$closure")->GetObjectValue());
		if (closureVariable != nullptr)
			return *closureVariable;
		throw RuntimeErrorException("No closure variable with id " + id);
	}
	else {
		if (!lvalue.GetObjectValue()->HasProperty(id))
			lvalue.GetObjectValue()->Set(id, Undefined());
		return *lvalue.GetObjectValue()->GetValue(id);
	}
}

Value* interpreter::Object_get(Value& rvalue, std::string id) {
	if (rvalue.GetObjectValue()->HasProperty("$closure")) {
		Value* closureVariable = LocalLookUp(id, rvalue.GetObjectValue()->GetValue("$closure")->GetObjectValue());
		if (closureVariable != nullptr)
			return closureVariable;
	}
	else {
		if (rvalue.GetObjectValue()->HasProperty(id))
			return rvalue.GetObjectValue()->GetValue(id);
	}
	return nullptr;
}

Value& interpreter::Object_set_brackets(Value& lvalue, Value& expr) {
	if (!lvalue.GetObjectValue()->HasProperty(expr))
		lvalue.GetObjectValue()->Set(expr, Undefined());
	return *lvalue.GetObjectValue()->GetValue(expr);
}

Value* interpreter::Object_get_brackets(Value& rvalue, Value& expr) {
	if (rvalue.GetObjectValue()->HasProperty(expr))
		return rvalue.GetObjectValue()->GetValue(expr);
	return nullptr;
}

