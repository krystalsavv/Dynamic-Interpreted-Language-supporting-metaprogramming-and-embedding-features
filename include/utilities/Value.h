#pragma once

#include <variant>
#include <string>
#include <iostream>
#include <cassert>

class Object;

#define variant_t std::variant<bool, double, std::string, Object*> 

class Value
{

	variant_t variant;

public:
	Value() = default;
	Value( bool value);
	Value( double value);
	Value(std::string value);
	Value( const char * value);
	Value( Object* value);

	bool& GetBoolValue();
	double& GetNumberValue();
	std::string& GetStringValue();
	Object* GetObjectValue();
	
	bool toBool();

	bool isBool();
	bool isNumber();
	bool isString();
	bool isObject();

	void PrintValue();

	//template set
	template <class T>
	void  Set(T value) {
		variant =value;
	}

	template <>
	void  Set<const char *>(const char * value) {
		variant = std::string(value);
	}

	//template <>
	//void  Set<Value>(Value value) {
	//	variant = std::string(value);
	//}



	//overloads
	Value operator+(Value& right);
	Value operator-(Value& right);
	Value operator*(Value& right);
	Value operator/(Value& right);
	Value operator%(Value& right);
	//prefix
	Value& operator++();
	//postfix
	Value operator++(int);
	//prefix
	Value& operator--();
	//postfix
	Value operator--(int);
	Value operator-();

	//equals
	Value operator>(Value& right);
	Value operator>=(Value& right);
	Value operator<(Value& right);
	Value operator<=(Value& right);

	//Value operator>(const Value& right) const;
	//Value operator>=(const Value& right) const;
	//Value operator<(const Value& right) const;
	//Value operator<=(const Value& right) const;


	Value operator==(Value& right);
	Value operator!=(Value& right);

	Value operator==(const Value& right) const;
	Value operator!=(const Value& right) const;

	//logical
	Value operator&&(Value& right);
	Value operator||(Value& right);
	Value operator!();

};

