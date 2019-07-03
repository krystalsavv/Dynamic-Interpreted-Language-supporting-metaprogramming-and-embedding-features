#include "utilities/EnvironmentHolder.h"

using namespace interpreter;

EnvironmentHolder* EnvironmentHolder::getInstance() {
	if (!envHolder)
		envHolder = new EnvironmentHolder();
	return envHolder;
}

void EnvironmentHolder::SetCurrentEnv(Environment* env) {
	currentEnv->DecreaseReferenceCounter();
	env->IncreaseReferenceCounter();
	currentEnv = env;
}

Environment* EnvironmentHolder::GetCurrentEnv() {
	return currentEnv;
}


void EnvironmentHolder::SetGlobalEnv(Environment* env) {
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
	return nestedBlock == 0;
}

unsigned int  EnvironmentHolder::GetNestedBlock() {
	return nestedBlock;
}

void  EnvironmentHolder::IncrementNestedBlock() {
	++nestedBlock;
}

void  EnvironmentHolder::DecrementNestedBlock() {
	--nestedBlock;
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
	if (GlobalLookUp(str) != nullptr)
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

	//EnvironmentHolder::getInstance()->DecrementNestedBlock();
}

BlockEnvironment* interpreter::SliceEnvironment(Environment* previous) {
	BlockEnvironment* blockEnv = new BlockEnvironment();
	blockEnv->Set("$previous", previous);
	return blockEnv; 
	//if (EnvironmentHolder::getInstance()->GetNestedBlock() == 0)
	//EnvironmentHolder::getInstance()->SetGlobalEnv(blockEnv);
}


void interpreter::InsertFunctionDefinition(std::string id, ASTnode* node) {
	// TODO: lookup (if already exists ERROR)
	EnvironmentHolder::getInstance()->GetCurrentEnv()->Set(id, node);
	node->Set("$closure", EnvironmentHolder::getInstance()->GetCurrentEnv());
	node->Set("$global", EnvironmentHolder::getInstance()->GetGlobalEnv());
	BlockEnvironment* block = SliceEnvironment(EnvironmentHolder::getInstance()->GetCurrentEnv());
	EnvironmentHolder::getInstance()->SetCurrentEnv(block);
}

void interpreter::InsertLvalue(std::string id, const Value& value) {
	// TODO: lookuo maybe here
	EnvironmentHolder::getInstance()->GetCurrentEnv()->Set(id, value);
}

Environment* interpreter::LocalLookUp(std::string id, Environment* envIterator) {		// kai tous environment pointers tous epistrefei ws values 
	 while (envIterator->HasProperty("$previous")) {
		if (envIterator->HasProperty(id)) {
			return envIterator;
		}
		envIterator = envIterator->GetValue("$previous")->GetObjectValue();
	}
	assert(envIterator->HasProperty("$outer"));
	if (envIterator->HasProperty(id)) {					// for the last environment node, which has an $outer pointer (not a $previous)
		return envIterator;
	}
	return nullptr;
}


 static void LocalLookUpForNormal(std::string id,  Environment*& envIterator) {		// kai tous environment pointers tous epistrefei ws values 
	 while (envIterator->HasProperty("$previous")) {
		 if (envIterator->HasProperty(id)) {
			 return;		// return envIterator;
		 }
		 envIterator = envIterator->GetValue("$previous")->GetObjectValue();
	 }
	 assert(envIterator->HasProperty("$outer"));
	 // return envIterator;
 }


 Environment* interpreter::NormalLookUp(std::string id) {
	Environment* envIterator = EnvironmentHolder::getInstance()->GetCurrentEnv();
	while (true) {
		if (envIterator->HasProperty("$outer") && envIterator->GetValue("$outer")->GetObjectValue() == nullptr)
			break;
		LocalLookUpForNormal(id, envIterator);
		//envIterator = LookupLocal_help(id, envIterator);
		if (envIterator->HasProperty(id)) {
			return envIterator;
		}
	}
	return nullptr;
}

 Environment* interpreter::GlobalLookUp(std::string id, Environment* envIterator ) {
	 return LocalLookUp(id, envIterator);
 }