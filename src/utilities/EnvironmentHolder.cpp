#include "utilities/EnvironmentHolder.h"

using namespace interpreter;

EnvironmentHolder* EnvironmentHolder::getInstance() {
	if (!envHolder)
		envHolder = new EnvironmentHolder();
	return envHolder;
}

void EnvironmentHolder::SetCurrentEnv(Object* env) {
	currentEnv = env;
}

Object* EnvironmentHolder::GetCurrentEnv() {
	return currentEnv;
}


void EnvironmentHolder::SetGlobalEnv(Object* env) {
	globalEnv = env;
}

Object* EnvironmentHolder::GetGlobalEnv() {
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
}

void interpreter::CreateFunctionEnvironment() {
	FunctionEnvironment* funcEnv = new FunctionEnvironment();
	funcEnv->Set("$outer", EnvironmentHolder::getInstance()->GetCurrentEnv());				// TODO: WRONG (prepei na vazei to func closure kai oxi curr ws outer)
	EnvironmentHolder::getInstance()->SetCurrentEnv(funcEnv);
}

void interpreter::CreateBlockEnvironment() {
	BlockEnvironment* blockEnv = new BlockEnvironment();
	blockEnv->Set("$outer", EnvironmentHolder::getInstance()->GetCurrentEnv());
	EnvironmentHolder::getInstance()->SetCurrentEnv(blockEnv);
	EnvironmentHolder::getInstance()->IncrementNestedBlock();
}

void interpreter::LeaveBlockEnvironment() {
	EnvironmentHolder::getInstance()->DecrementNestedBlock();
}

void interpreter::InsertFunctionDefinition(std::string id, ASTnode* node) {
	EnvironmentHolder::getInstance()->GetCurrentEnv()->Set(id, node);
}

void interpreter::SliceEnvironment() {
	BlockEnvironment* blockEnv = new BlockEnvironment();
	blockEnv->Set("$previous", EnvironmentHolder::getInstance()->GetCurrentEnv());
	EnvironmentHolder::getInstance()->SetCurrentEnv(blockEnv);											// NOT SURE CurrentEnv is the sliced one
	if (EnvironmentHolder::getInstance()->GetNestedBlock() == 0)
		EnvironmentHolder::getInstance()->SetGlobalEnv(blockEnv);
}

