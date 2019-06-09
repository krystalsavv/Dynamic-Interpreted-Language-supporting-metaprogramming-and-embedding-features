//
// Created by dimitris on 8/6/2019.
//

#ifndef CS540_AST_H
#define CS540_AST_H

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

#endif
