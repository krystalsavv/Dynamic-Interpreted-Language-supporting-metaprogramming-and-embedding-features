#include "utilities/Environment.h"

Environment::Environment()
{
	currentEnv = nullptr;
}

void Environment::SetCurrentEnv(Object* env) {
	currentEnv = env;
}

Object* Environment::GetCurrentEnv() {
	return currentEnv;
}

