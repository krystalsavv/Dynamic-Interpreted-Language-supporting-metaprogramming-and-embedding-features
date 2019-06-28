#include "utilities/AST.h"

using namespace interpreter;

AST::AST(ASTnode* root) {
	this->root = root;
}

ASTnode* AST::GetRoot() {
	return root;
}

void AST::SetRoot(ASTnode* root) {
	this->root = root;
}

void AST::Print() {
	std::cout << *root; 
}

//void AST::Evaluate() {
//	//TODO
//	// Value tmp = Evaluator::Evaluate(root);
//}