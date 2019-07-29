#include <iostream>
#include <string>
#include "utilities/Value.h"

using namespace interpreter;

int main() {

	int x = 5,y=3;

	ValueReference val (&x);
	ValueReference val2(&y);

	ValueReference result  = val + val2;

	Value value = val;
	Value value2 = val2;

	std::cout << value * value2;

}