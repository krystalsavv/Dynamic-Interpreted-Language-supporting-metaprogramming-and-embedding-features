#pragma once
#include "utilities/Object.h"
#include "utilities/Evaluator.h"

class AST {
	Object* root = nullptr;

public:
	AST() = default;
	AST(Object* root);

	Object* GetRoot();
	void SetRoot(Object* root);

	void Print();
	void Evaluate();
};