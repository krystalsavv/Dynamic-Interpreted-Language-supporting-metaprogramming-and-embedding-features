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
		bool isGlobalScope();

		static unsigned int GetNestedBlock();
		static void IncrementNestedBlock();
		static void DecrementNestedBlock();

		void PrintEnvironmentChain();
		void PrintGlobalEnvironment();
	};

	void InitGlobalEnvironment();
	bool hasCollisionWithLibFunc(std::string str);
	void CreateFunctionEnvironment();
	void CreateBlockEnvironment();
	void LeaveBlockEnvironment(); 
	BlockEnvironment* SliceEnvironment(Environment* previous);
	void InsertFunctionDefinition(std::string id, ASTnode* node);
	

	void InsertFunctionDefinition(std::string id, ASTnode* node);
	void InsertLvalue(std::string id, const Value& value);
	Environment* LocalLookUp(std::string id, Environment* envIterator = EnvironmentHolder::getInstance()->GetCurrentEnv());
	Environment* NormalLookUp(std::string id);
	Environment* GlobalLookUp(std::string id, Environment* envIterator = EnvironmentHolder::getInstance()->GetGlobalEnv());
}
