#pragma once
#include "utilities/Object.h"

class AST {
	Object* root = nullptr;
	AST(Object* root);
};