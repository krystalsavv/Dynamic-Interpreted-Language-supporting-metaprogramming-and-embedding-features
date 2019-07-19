#pragma once
#include "utilities/Object.h"
#include "utilities/AST.h"
#include "utilities/Exceptions.h"

namespace interpreter {

	using BlockEnvironment = Object;
	using FunctionEnvironment =	Object;
	using ClosureEnvironment = Object;
	using Environment = Object;

	class EnvironmentHolder
	{
		inline static EnvironmentHolder* envHolder = nullptr;

		Environment* currentEnv = nullptr;
		Environment* globalEnv = nullptr;
		EnvironmentHolder() = default;
		~EnvironmentHolder();

	public:
		static EnvironmentHolder* getInstance();
		static void destroyInstance();
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
	void CreateFunctionEnvironment(ASTnode* funcClosure);
	void LeaveFunctionEnvironment(Environment* oldCurr);
	void CreateBlockEnvironment();
	void LeaveBlockEnvironment(); 
	BlockEnvironment* SliceEnvironment(Environment* previous);

	Value* InsertFunctionDefinition(std::string id, ASTnode* node);
	void ExpressionfunctionDefinition(ASTnode* node);
	Value* InsertLvalue(std::string id, const Value& value, Environment* envIterator = EnvironmentHolder::getInstance()->GetCurrentEnv());
	Value* LocalLookUp(std::string id, Environment* envIterator = EnvironmentHolder::getInstance()->GetCurrentEnv());
	Value* NormalLookUp(std::string id, Environment* envIterator = EnvironmentHolder::getInstance()->GetCurrentEnv());
	Value* GlobalLookUp(std::string id, Environment* envIterator = EnvironmentHolder::getInstance()->GetGlobalEnv());

	void ClearEnvironment();
}
