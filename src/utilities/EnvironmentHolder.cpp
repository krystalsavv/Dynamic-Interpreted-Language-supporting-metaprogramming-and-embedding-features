#include "utilities/EnvironmentHolder.h"

EnvironmentHolder* EnvironmentHolder::envHolder = nullptr;

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
