#include "utilities/AST.h"

using namespace interpreter;

AST::AST(ASTnode* root) {
	this->root = root;
}

AST::~AST(){
	// std::cout << "the ast is gone" << std::endl;
}


ASTnode* AST::GetRoot() {
	return root;
}

void AST::SetRoot(ASTnode* root) {
	assert(root);
	this->root = root;
	root->IncreaseReferenceCounter();
}

void AST::Print() {
	if(root)
		std::cout << *root;
}


void interpreter::DestroyAst(Object* node) {
	if (!node) return;
	for (auto& pair : node->GetMap()) {
		if (pair.second.isObject() && pair.second.GetObjectValue())
			DestroyAst(pair.second.GetObjectValue());
	}
	delete node;
}