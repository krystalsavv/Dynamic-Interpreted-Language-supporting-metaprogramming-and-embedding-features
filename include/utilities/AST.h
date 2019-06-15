#pragma once
#include "utilities/Object.h"

class AST {
	Object* root = nullptr;

public:
	AST(Object* root);
};