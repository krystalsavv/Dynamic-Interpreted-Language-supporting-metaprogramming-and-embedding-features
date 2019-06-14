#pragma once

#include <variant>
#include <string>
#include <iostream>

class Object;

#define variant_t std::variant<bool, double, std::string, Object*> 

class Value
{

	variant_t variant;

public:
	Value() = default;
	Value( bool value);
	Value( double value);
	Value( std::string value);
	Value( Object* value);

	void Set(bool value);
	void Set(double value);
	void Set(std::string value);
	void Set(Object* value);

	bool& GetBoolValue();
	double& GetNumberValue();
	std::string& GetStringValue();
	Object* GetObjectValue();

	bool isBool();
	bool isNumber();
	bool isString();
	bool isObject();

	void PrintValue();
	
};

