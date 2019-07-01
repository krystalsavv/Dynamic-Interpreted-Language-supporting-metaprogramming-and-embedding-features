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
	EnvironmentHolder::getInstance()->SetCurrentEnv(blockEnv);
	EnvironmentHolder::getInstance()->SetGlobalEnv(blockEnv);
	// TODO: Insert/add libfunctions in current scope
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
	Environment* envPointer = EnvironmentHolder::getInstance()->GetCurrentEnv();
	while (envPointer->HasProperty("$previous")) {
		envPointer = envPointer->GetValue("$previous").GetObjectValue();
	}

	Environment* nextEnv;
	if (envPointer->GetValue("$sliced").GetBoolValue()){
		nextEnv = SliceEnvironment(envPointer->GetValue("$outer").GetObjectValue());
	}
	else {
		nextEnv = envPointer->GetValue("$outer").GetObjectValue();
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
	EnvironmentHolder::getInstance()->GetCurrentEnv()->Set(id, node);
	node->Set("$closure", EnvironmentHolder::getInstance()->GetCurrentEnv());
	node->Set("$global", EnvironmentHolder::getInstance()->GetGlobalEnv());
}



