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
	Value* InsertLvalue(std::string id, const Value& value, Environment* envIterator = EnvironmentHolder::getInstance()->GetCurrentEnv());
	Value* LocalLookUp(std::string id, Environment* envIterator = EnvironmentHolder::getInstance()->GetCurrentEnv());
	Value* NormalLookUp(std::string id, Environment* envIterator = EnvironmentHolder::getInstance()->GetCurrentEnv());
	Value* GlobalLookUp(std::string id, Environment* envIterator = EnvironmentHolder::getInstance()->GetGlobalEnv());

	Value* LvalueVarActions(std::string id, bool insertFlag = true, Environment* envIterator = EnvironmentHolder::getInstance()->GetCurrentEnv());
	Value* LvalueLocalVarActions(std::string id, bool insertFalg = true, Environment* envIterator = EnvironmentHolder::getInstance()->GetCurrentEnv());
	Value* GlobalVarActions(std::string id, Environment* envIterator = EnvironmentHolder::getInstance()->GetGlobalEnv());
	Value* LvalueFuncDefActions(std::string id, ASTnode* node);
	Value* RvalueVarActions(std::string id);
	Value* RvalueLocalVarActions(std::string id);

}
