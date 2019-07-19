#include "utilities/AST.h"

using namespace interpreter;

AST::AST(ASTnode* root) {
	this->root = root;
}

AST::~AST(){
	std::cout << "the ast is gone" << std::endl;
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

void interpreter::TraverseAndClearAst(Object* node){
	//if (!node) return;
	assert(node);
	if (node->HasProperty("$global")) {
		assert(node->HasProperty("$closure"));
		node->GetValue("$global")->GetObjectValue()->DecreaseReferenceCounter();
		node->GetValue("$closure")->GetObjectValue()->DecreaseReferenceCounter();
	}
	for (auto& pair : node->GetMap()) {
		if (pair.first.isString() && (pair.first.GetStringValue() == "$global" || pair.first.GetStringValue() == "$closure"))
			continue;
		else if(pair.second.isObject() && pair.second.GetObjectValue())
			TraverseAndClearAst(pair.second.GetObjectValue());
	}
}

void interpreter::DestroyAst(Object* node) {
	if (!node) return;
	for (auto& pair : node->GetMap()) {
		if (pair.first.isString() && (pair.first.GetStringValue() == "$global" || pair.first.GetStringValue() == "$closure"))
			continue;
		else if (pair.second.isObject() && pair.second.GetObjectValue())
			DestroyAst(pair.second.GetObjectValue());
	}
	delete node;
}