#pragma once
#include "utilities/Object.h"

#define BlockEnvironment	Object
#define FunctionEnvironment	Object
#define ClosureEnvironment	Object

class Environment
{
public:
	Environment();
	//~Environment();

	void SetCurrentEnv(Object* env);

	Object* GetCurrentEnv();


private:
	Object* currentEnv;
};

