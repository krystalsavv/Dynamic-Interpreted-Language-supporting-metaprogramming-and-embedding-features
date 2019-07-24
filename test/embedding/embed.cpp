#include <iostream>
#include <string>
#include "utilities/ValueReference.h"

using namespace interpreter;

int main() {
	int x = 5,y=3;
	ValueReference val (&x);
	ValueReference val2(&y);
	ValueReference result ( val + val2) ;
	//std::cout << (result++).toString();
	++result;
	++result;
	//std::cout << result.toString() << std::endl;
	(result)++;
	//-result;
	std::cout << result.toString();
	//int o = *(val2->GetInteger());
	//std::cout << o;
}
