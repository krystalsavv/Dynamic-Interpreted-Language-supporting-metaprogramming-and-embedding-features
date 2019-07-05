#include "utilities/EnvironmentHolder.h"

using namespace interpreter;

#define CURRENT_ENV 

EnvironmentHolder* EnvironmentHolder::getInstance() {
	if (!envHolder)
		envHolder = new EnvironmentHolder();
	return envHolder;
}

void EnvironmentHolder::SetCurrentEnv(Environment* env) {
	if(currentEnv != nullptr )
		currentEnv->DecreaseReferenceCounter();
	env->IncreaseReferenceCounter();
	currentEnv = env;
}

Environment* EnvironmentHolder::GetCurrentEnv() {
	return currentEnv;
}


void EnvironmentHolder::SetGlobalEnv(Environment* env) {
	if(globalEnv != nullptr)
		globalEnv->DecreaseReferenceCounter();
	env->IncreaseReferenceCounter();
	globalEnv = env;
}

Environment* EnvironmentHolder::GetGlobalEnv() {
	return globalEnv;
}

void EnvironmentHolder::PrintEnvironmentChain() {
	std::cout << *currentEnv;
}

void EnvironmentHolder::PrintGlobalEnvironment()
{
	std::cout << *globalEnv;
}

bool EnvironmentHolder::isGlobalScope() {
	return currentEnv == globalEnv;
}

// -----------

void interpreter::InitGlobalEnvironment() {
	BlockEnvironment* blockEnv = new BlockEnvironment();
	blockEnv->Set("$outer", (Object*)nullptr);
	EnvironmentHolder::getInstance()->SetCurrentEnv(blockEnv);
	EnvironmentHolder::getInstance()->SetGlobalEnv(blockEnv);
	// TODO: Insert/add libfunctions in current scope
}

bool interpreter::hasCollisionWithLibFunc(std::string str)
{
	if(str=="print" || str == "typeof" || str == "object_keys" || str == "object_size" || str == "eval")
		return true;
	return false;
}

void interpreter::CreateFunctionEnvironment() {
	FunctionEnvironment* funcEnv = new FunctionEnvironment();
	funcEnv->Set("$outer", EnvironmentHolder::getInstance()->GetCurrentEnv());				// TODO: WRONG (prepei na vazei to func closure kai oxi curr ws outer)
	EnvironmentHolder::getInstance()->SetCurrentEnv(funcEnv);
}

void interpreter::CreateBlockEnvironment() {
	BlockEnvironment* blockEnv = new BlockEnvironment();
	blockEnv->Set("$outer", EnvironmentHolder::getInstance()->GetCurrentEnv());
	blockEnv->Set("$sliced", false);
	EnvironmentHolder::getInstance()->SetCurrentEnv(blockEnv);
}

void interpreter::LeaveBlockEnvironment() {
	Environment* envIterator = EnvironmentHolder::getInstance()->GetCurrentEnv();
	while (envIterator->HasProperty("$previous")) {
		envIterator = envIterator->GetValue("$previous")->GetObjectValue();
	}

	Environment* nextEnv;
	if (envIterator->GetValue("$sliced")->GetBoolValue()){
		nextEnv = SliceEnvironment(envIterator->GetValue("$outer")->GetObjectValue());
	}
	else {
		nextEnv = envIterator->GetValue("$outer")->GetObjectValue();
	}
	EnvironmentHolder::getInstance()->SetCurrentEnv(nextEnv);
}

BlockEnvironment* interpreter::SliceEnvironment(Environment* previous) {
	BlockEnvironment* blockEnv = new BlockEnvironment();
	blockEnv->Set("$previous", previous);
	return blockEnv; 
}

Value* interpreter::InsertFunctionDefinition(std::string id, ASTnode* node) {
	EnvironmentHolder::getInstance()->GetCurrentEnv()->Set(id, node);
	Value* value = EnvironmentHolder::getInstance()->GetCurrentEnv()->GetValue(id);
	node->Set("$closure", EnvironmentHolder::getInstance()->GetCurrentEnv());
	node->Set("$global", EnvironmentHolder::getInstance()->GetGlobalEnv());
	BlockEnvironment* block = SliceEnvironment(EnvironmentHolder::getInstance()->GetCurrentEnv());
	if (EnvironmentHolder::getInstance() -> isGlobalScope()) {
		EnvironmentHolder::getInstance()->SetGlobalEnv(block);
	}
	EnvironmentHolder::getInstance()->SetCurrentEnv(block);
	return value;
}

Value* interpreter::InsertLvalue(std::string id, const Value& value, Environment* envIterator) {
	// TODO: lookuo maybe here
	envIterator->Set(id, value);
	return envIterator->GetValue(id);
}

Value* interpreter::LocalLookUp(std::string id, Environment* envIterator) {		// kai tous environment pointers tous epistrefei ws values 
	 while (envIterator->HasProperty("$previous")) {
		 Value* value = envIterator->GetValue(id);
		 if (value != nullptr)
			return value;
		envIterator = envIterator->GetValue("$previous")->GetObjectValue();
	}
	assert(envIterator->HasProperty("$outer"));
	Value* value = envIterator->GetValue(id);
	if (value != nullptr) 	// for the last environment node, which has an $outer pointer (not a $previous)
		return value;
	return nullptr;
}

 static Environment* LocalLookUpForNormal(std::string id,  Environment* envIterator) {		// kai tous environment pointers tous epistrefei ws values 
	 while (envIterator->HasProperty("$previous")) {
		 if (envIterator->HasProperty(id)) {
			 return envIterator;
		 }
		 envIterator = envIterator->GetValue("$previous")->GetObjectValue();
	 }
	 assert(envIterator->HasProperty("$outer"));
	 return envIterator;
 }

 Value* interpreter::NormalLookUp(std::string id, Environment* envIterator) {
	while (envIterator!=nullptr){
		
		envIterator = LocalLookUpForNormal(id, envIterator);
		Value* value = envIterator->GetValue(id);
		if(value != nullptr)
			return value;
		envIterator = envIterator->GetValue("$outer")->GetObjectValue();

	}
	return nullptr;
}

 Value* interpreter::GlobalLookUp(std::string id, Environment* envIterator ) {
	 return LocalLookUp(id, envIterator);
 }

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

 Value* interpreter::LocalVarActions(std::string id, bool insertFlag, Environment* envIterator) {
	 Value* value = LocalLookUp(id, envIterator);
	 if ((value != nullptr && !hasCollisionWithLibFunc(id)) ||
		 (value != nullptr && hasCollisionWithLibFunc(id) && EnvironmentHolder::getInstance()->isGlobalScope()))
		 return value;
	 else if ((value != nullptr && hasCollisionWithLibFunc(id)) || !insertFlag)
		 return nullptr;
	 else if(insertFlag) {
		 return InsertLvalue(id, Value(), envIterator);
	 }
 }

Value* interpreter::GlobalVarActions(std::string id, Environment* envIterator) {
	return GlobalLookUp(id, envIterator);
}

Value* interpreter::LvalueFuncDefActions(std::string id, ASTnode* node) {
	Value* value = LocalLookUp(id);
	if (value != nullptr || hasCollisionWithLibFunc(id))
		return nullptr;
	return InsertFunctionDefinition(id, node);
}

Value* interpreter::RvalueVarActions(std::string id)
{
	Value* value = NormalLookUp(id);
	if (value != nullptr)
		return value;
	else
		return InsertLvalue(id, Value());
}