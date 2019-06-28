#pragma once
#include "utilities/Object.h"

namespace interpreter {

	using ASTnode = Object;

	class AST {
		Object* root = nullptr;

	public:
		AST() = default;
		AST(ASTnode* root);

		ASTnode* GetRoot();
		void SetRoot(ASTnode* root);

		void Print();
		//void Evaluate();
	};
}