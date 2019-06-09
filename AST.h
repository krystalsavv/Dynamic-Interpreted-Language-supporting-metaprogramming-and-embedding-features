#pragma once
#include <iostream>
#include <string>
#include "Object.h"

class AST : Object {
    std::string type;

public:

    std::string& getType();

    void setType(std::string&);



};

class ASTVisitor{

};

