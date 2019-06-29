#pragma once
#include "utilities/Object.h"
#include "utilities/AST.h"

namespace interpreter {

	using BlockEnvironment = Object;
	using FunctionEnvironment =	Object;
	using ClosureEnvironment = Object;

	class EnvironmentHolder
	{
		inline static EnvironmentHolder* envHolder = nullptr;

		inline static unsigned int nestedBlock = 0;
		Object* currentEnv = nullptr;
		Object* storedEnv = nullptr;
		Object* globalEnv = nullptr;
		EnvironmentHolder() = default;

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
	};

	void InitGlobalEnvironment();
	void CreateFunctionEnvironment();
	void CreateBlockEnvironment();
	void LeaveBlockEnvironment(); 
	void InsertFunctionDefinition(std::string id, ASTnode* node);
	void SliceEnvironment();
	
}
