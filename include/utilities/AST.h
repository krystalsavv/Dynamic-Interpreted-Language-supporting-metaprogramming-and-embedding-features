#pragma once
#include "utilities/Object.h"

namespace interpreter {

	using ASTnode = Object;

	class AST {
		ASTnode* root = nullptr;

	public:
		AST() = default;
		AST(ASTnode* root);
		~AST();
		ASTnode* GetRoot();
		void SetRoot(ASTnode* root);

		void Print();
		//void Evaluate();
	};
	void TraverseAndClearAst(Object* node);
	void DestroyAst(Object* node);
}