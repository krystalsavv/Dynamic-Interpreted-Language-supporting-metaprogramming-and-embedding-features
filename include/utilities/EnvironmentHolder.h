#pragma once
#include "utilities/Object.h"
#include "utilities/AST.h"

namespace interpreter {

	using BlockEnvironment = Object;
	using FunctionEnvironment =	Object;
	using ClosureEnvironment = Object;
	using Environment = Object;

	class EnvironmentHolder
	{
		inline static EnvironmentHolder* envHolder = nullptr;

		inline static unsigned int nestedBlock = 0;
		Environment* currentEnv = nullptr;
		Environment* storedEnv = nullptr;
		Environment* globalEnv = nullptr;
		EnvironmentHolder() = default;

	public:
		static EnvironmentHolder* getInstance();

		void SetCurrentEnv(Environment* env);
		Environment* GetCurrentEnv();

		void SetGlobalEnv(Environment* env);
		Environment* GetGlobalEnv();

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
	BlockEnvironment* SliceEnvironment(Environment* previous);
	void InsertFunctionDefinition(std::string id, ASTnode* node);
	
}
