#include "utilities/AST.h"

AST::AST(Object* root) {
	this->root = root;
}


Object* AST::GetRoot() {
	return root;
}


void AST::SetRoot(Object* root) {
	this->root = root;
}