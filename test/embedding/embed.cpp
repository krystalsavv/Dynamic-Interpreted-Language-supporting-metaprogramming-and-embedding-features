#include <iostream>
#include <string>
#include "interpreter/Interpreter.h"

using namespace interpreter;

/*
int main() {

	int x = 5,y=3;

	ValueReference val(&x);
	ValueReference val2(&y);

	ValueReference result  = val + val2;

	std::cout << "Result of ValueReference " << val << " and ValueReference " << val2 << " is " << result << std::endl;

	std::cout << "Change value of x from wrapper val to 15" << std::endl;
	val.Set(15);

	std::cout << "x: " << x << " val: " << val << std::endl;

	std::cout << "Change value of x from wrapper val same as " << val2 << std::endl;
	val.Set(val2);

	Value value = val;
	Value value2 = val2;

	std::cout << "Result of multiplication for value " << value << " and value " << value2 << " is " << value * value2 << std::endl;

}
*/

int main() {

	Interpreter inter;

	double x = 10, y = 50;

	ValueReference val(&x);
	ValueReference val2(&y);

	inter.Initialize();

	inter.AddVariable("x",val);
	inter.AddVariable("y",val2);

	x = 15;

	inter.Execute("function f(i,j) { " 
							"y = 20;"
							"return y+x;"
						 "}"
						"print(f(x,y));"	
	);

	std::cout << std::endl << "The c++ var y has value: " << y << std::endl;
}