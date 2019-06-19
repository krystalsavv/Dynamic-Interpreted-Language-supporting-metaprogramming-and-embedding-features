#pragma once
#include "utilities/Object.h"

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

