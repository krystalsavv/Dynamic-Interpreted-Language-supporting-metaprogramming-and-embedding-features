#pragma once
#include "utilities/EnvironmentHolder.h"

namespace interpreter {
	static Object* argTable = nullptr;

	Value* LvalueVarActions(std::string id, bool insertFlag = true, Environment* envIterator = EnvironmentHolder::getInstance()->GetCurrentEnv());
	Value* LvalueLocalVarActions(std::string id, bool insertFlag = true, Environment* envIterator = EnvironmentHolder::getInstance()->GetCurrentEnv());
	Value* GlobalVarActions(std::string id, Environment* envIterator = EnvironmentHolder::getInstance()->GetGlobalEnv());
	Value* LvalueFuncDefActions(std::string id, ASTnode* node);
	Value* RvalueVarActions(std::string id, bool insertFlag = true);
	Value* RvalueLocalVarActions(std::string id, bool insertFlag = true);

	Value& CallerEnvironmentActions(Value& funcdefNode, bool isFunctor = false);
	void AddPositionalParamsToEnvironment(Object* idList, Object* argTable);
	void AddNamedParamsToEnvironment(Object& idList_withoutIndex, Object* argTable);
	Object* AddLvalueAsFirstArg(Value& lvalue);

	Value& Object_set(Value& lvalue, std::string id);
	Value* Object_get(Value& rvalue, std::string id);
	Value& Object_set_brackets(Value& lvalue, Value& expr);
	Value* Object_get_brackets(Value& rvalue, Value& expr);

}