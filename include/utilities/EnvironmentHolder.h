#pragma once
#include "utilities/Object.h"

#define BlockEnvironment	Object
#define FunctionEnvironment	Object
#define ClosureEnvironment	Object

class EnvironmentHolder
{
	static EnvironmentHolder* envHolder;
	Object* currentEnv = nullptr;
	Object* globalEnv = nullptr;
	EnvironmentHolder();
public:

	static EnvironmentHolder* getInstance();

	void SetCurrentEnv(Object* env);
	Object* GetCurrentEnv();

	void SetGlobalEnv(Object* env);
	Object* GetGlobalEnv();

private:
	
};

