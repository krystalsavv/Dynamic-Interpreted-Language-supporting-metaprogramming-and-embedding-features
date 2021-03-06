#include "utilities/EnvironmentHolder.h"

using namespace interpreter;

#define CURRENT_ENV 

EnvironmentHolder::~EnvironmentHolder(){
	currentEnv = nullptr;
	globalEnv = nullptr;
}

EnvironmentHolder* EnvironmentHolder::getInstance() {
	if (!envHolder)
		envHolder = new EnvironmentHolder();
	return envHolder;
}

void EnvironmentHolder::destroyInstance(){
	assert(envHolder);
	delete envHolder;
}

void EnvironmentHolder::SetCurrentEnv(Environment* env) {
	env->IncreaseReferenceCounter();
	Environment* oldCurrent = currentEnv;
	currentEnv = env;
	if(oldCurrent != nullptr )
		oldCurrent->DecreaseReferenceCounter();
}

Environment* EnvironmentHolder::GetCurrentEnv() {
	return currentEnv;
}


void EnvironmentHolder::SetGlobalEnv(Environment* env) {
	env->IncreaseReferenceCounter();
	Environment* oldGlobal = globalEnv;
	globalEnv = env;
	if (oldGlobal != nullptr)
		oldGlobal->DecreaseReferenceCounter();
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

void insertLibFunctions() {
	Environment* curr =TemporarilySaveEnvironment(EnvironmentHolder::getInstance()->GetCurrentEnv()); 
	curr->Set("print", new ASTnode("type", "print"));
	curr->Set("typeof", new ASTnode("type", "typeof")); 
	curr->Set("object_keys", new ASTnode("type", "object_keys"));
	curr->Set("object_size", new ASTnode("type", "object_size"));
	curr->Set("eval", new ASTnode("type", "eval"));
	curr->Set("toString", new ASTnode("type", "toString"));
	DecreaseTemporarilySavedEnvironment(curr);
}

void interpreter::InitGlobalEnvironment() {
	CreateBlockEnvironment();
	EnvironmentHolder::getInstance()->SetGlobalEnv(EnvironmentHolder::getInstance()->GetCurrentEnv());
	insertLibFunctions();
}

bool interpreter::hasCollisionWithLibFunc(std::string str)
{
	if(str=="print" || str == "typeof" || str == "object_keys" || str == "object_size" || str == "eval" || str == "toString")
		return true;
	return false;
}

void interpreter::CreateFunctionEnvironment(ASTnode* funcClosure) {
	FunctionEnvironment* funcEnv = new FunctionEnvironment();
	funcEnv->Set("$sliced", false);
	funcEnv->Set("$outer", funcClosure);
	EnvironmentHolder::getInstance()->SetCurrentEnv(funcEnv);
}

void interpreter::LeaveFunctionEnvironment(Environment* oldCurrent) {
	EnvironmentHolder::getInstance()->SetCurrentEnv(oldCurrent);
}

void interpreter::CreateBlockEnvironment() {
	BlockEnvironment* blockEnv = new BlockEnvironment();
	blockEnv->Set("$outer", EnvironmentHolder::getInstance()->GetCurrentEnv());
	blockEnv->Set("$sliced", false);
	EnvironmentHolder::getInstance()->SetCurrentEnv(blockEnv);
}

void interpreter::LeaveBlockEnvironment() {
	Environment* envIterator =TemporarilySaveEnvironment(EnvironmentHolder::getInstance()->GetCurrentEnv());
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
	nextEnv->IncreaseReferenceCounter();
	EnvironmentHolder::getInstance()->SetCurrentEnv(nextEnv);
	DecreaseTemporarilySavedEnvironment(nextEnv);
	DecreaseTemporarilySavedEnvironment(envIterator);
}

BlockEnvironment* interpreter::SliceEnvironment(Environment* previous) {
	BlockEnvironment* blockEnv = new BlockEnvironment();
	blockEnv->Set("$previous", previous);
	
	return blockEnv; 
}

Value* interpreter::InsertFunctionDefinition(std::string id, ASTnode* node) {
	Object* funcdef_obj = new Object(); 
	funcdef_obj->Set("funcdef", node); 
	funcdef_obj->Set("$closure", EnvironmentHolder::getInstance()->GetCurrentEnv());
	funcdef_obj->Set("$global", EnvironmentHolder::getInstance()->GetGlobalEnv());

	EnvironmentHolder::getInstance()->GetCurrentEnv()->Set(id, funcdef_obj);

	Value* value = EnvironmentHolder::getInstance()->GetCurrentEnv()->GetValue(id);
	
	if (EnvironmentHolder::getInstance()->GetCurrentEnv()->HasProperty("$outer")) {
		EnvironmentHolder::getInstance()->GetCurrentEnv()->Set("$sliced", true);
	}
	BlockEnvironment* block = TemporarilySaveEnvironment(SliceEnvironment(EnvironmentHolder::getInstance()->GetCurrentEnv()));
	if (EnvironmentHolder::getInstance() -> isGlobalScope()) {
		EnvironmentHolder::getInstance()->SetGlobalEnv(block);
	}
	EnvironmentHolder::getInstance()->SetCurrentEnv(block);

	DecreaseTemporarilySavedEnvironment(block);
	return value;
}



Object* interpreter::ExpressionfunctionDefinition(ASTnode* node) {
	Object* funcdef_obj = new Object();
	funcdef_obj->Set("funcdef", node);
	funcdef_obj->Set("$closure", EnvironmentHolder::getInstance()->GetCurrentEnv());
	funcdef_obj->Set("$global", EnvironmentHolder::getInstance()->GetGlobalEnv());

	if (EnvironmentHolder::getInstance()->GetCurrentEnv()->HasProperty("$outer")) {
		EnvironmentHolder::getInstance()->GetCurrentEnv()->Set("$sliced", true);
	}
	BlockEnvironment* block = TemporarilySaveEnvironment(SliceEnvironment(EnvironmentHolder::getInstance()->GetCurrentEnv()));
	if (EnvironmentHolder::getInstance()->isGlobalScope()) {
		EnvironmentHolder::getInstance()->SetGlobalEnv(block);
	}
	EnvironmentHolder::getInstance()->SetCurrentEnv(block);
	DecreaseTemporarilySavedEnvironment(block);

	return funcdef_obj;
}


Value* interpreter::InsertLvalue(std::string id, const Value& value, Environment* envIterator) {
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
	 return LocalLookUp(id, EnvironmentHolder::getInstance()->GetGlobalEnv());
 }

 Environment* interpreter::TemporarilySaveEnvironment(Environment* env) {
	 if(env)
		env->IncreaseReferenceCounter();
	 return env;
 }

 void interpreter::DecreaseTemporarilySavedEnvironment(Environment* env) {
	 if(env)
		env->DecreaseReferenceCounter();
 }

 void interpreter::ClearEnvironment(){
	 EnvironmentHolder::getInstance()->GetCurrentEnv()->DecreaseReferenceCounter();
	 EnvironmentHolder::getInstance()->GetGlobalEnv()->DecreaseReferenceCounter();
 }