#pragma once
#include "utilities/EnvironmentHolder.h"

namespace interpreter {
	extern Object* argTable;

	Value* LvalueVarActions(std::string id, bool insertFlag = true, Environment* envIterator = EnvironmentHolder::getInstance()->GetCurrentEnv());
	Value* LvalueLocalVarActions(std::string id, bool insertFlag = true, Environment* envIterator = EnvironmentHolder::getInstance()->GetCurrentEnv());
	Value* GlobalVarActions(std::string id, Environment* envIterator = EnvironmentHolder::getInstance()->GetGlobalEnv());
	Value* LvalueFuncDefActions(std::string id, ASTnode* node);
	Value* RvalueVarActions(std::string id, bool insertFlag = true);
	Value* RvalueLocalVarActions(std::string id, bool insertFlag = true);

	Value& CallerEnvironmentActions(Value& funcdefNode, const Value& that = (Object*)nullptr, bool isFunctor = false);
	void AddPositionalParamsToEnvironment(Object* idList);
	void AddNamedParamsToEnvironment(Object& idList_withoutIndex);
	void AddLvalueAsFirstArg(const Value& lvalue);

	Value& Object_set(Value& lvalue, std::string id);
	Value* Object_get(Value& rvalue, std::string id);
	Value& Object_set_brackets(Value& lvalue, Value& expr);
	Value* Object_get_brackets(Value& rvalue, Value& expr);

}