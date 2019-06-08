//
// Created by dimitris on 8/6/2019.
//

#ifndef CS540_AST_H
#define CS540_AST_H

#include <iostream>
#include <string>

class AST {

    std::string type;

public:

    //return type
    std::string& getType();

    //set type
    void setType(std::string&);



};

class ASTVisitor{

};

#endif //CS540_AST_H
