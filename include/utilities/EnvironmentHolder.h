#pragma once
#include "utilities/Object.h"

#define BlockEnvironment	Object
#define FunctionEnvironment	Object
#define ClosureEnvironment	Object

class EnvironmentHolder
{
	static EnvironmentHolder* envHolder;
	static unsigned int nestedBlock;
	Object* currentEnv = nullptr;
	Object* globalEnv = nullptr;
	EnvironmentHolder()=default;
public:

	static EnvironmentHolder* getInstance();

	void SetCurrentEnv(Object* env);
	Object* GetCurrentEnv();

	void SetGlobalEnv(Object* env);
	Object* GetGlobalEnv();

	static unsigned int GetNestedBlock();
	static void IncrementNestedBlock();
	static void DecrementNestedBlock();

	void PrintEnvironmentChain();
	void PrintGlobalEnvironment();

private:
	
};

