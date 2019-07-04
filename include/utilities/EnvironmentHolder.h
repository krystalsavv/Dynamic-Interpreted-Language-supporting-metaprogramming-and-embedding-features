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

	Value* InsertFunctionDefinition(std::string id, ASTnode* node);
	Value* InsertLvalue(std::string id, const Value& value);
	Value* LocalLookUp(std::string id, Environment* envIterator = EnvironmentHolder::getInstance()->GetCurrentEnv());
	Value* NormalLookUp(std::string id);
	Value* GlobalLookUp(std::string id, Environment* envIterator = EnvironmentHolder::getInstance()->GetGlobalEnv());

	Value* LvalueVarActions(std::string id);
	Value* LocalVarActions(std::string id);
	Value* GlobalVarActions(std::string id);
	Value* LvalueFuncDefActions(std::string id, ASTnode* node);
	Value* RvalueVarActions(std::string id);
}
