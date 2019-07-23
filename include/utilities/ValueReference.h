#pragma once
#include <variant>
#include <string>
#include <iostream>
#include <cassert>
#include <iostream> 
#include "utilities/Exceptions.h"

#define native_t std::variant<int*,double*,bool*,std::string*,void*> 

namespace interpreter {
	class ValueReference
	{
		native_t variant;
		std::string type;
	public:
		ValueReference();
		ValueReference(int* value);
		ValueReference(int value);
		ValueReference(double* value);
		ValueReference(double value);
		ValueReference(std::string* value);
		ValueReference(std::string value);
		ValueReference(bool* value);
		ValueReference(bool value);
		ValueReference(void* value, std::string type = "class");
		ValueReference(const ValueReference& value);



		const int* GetInteger() const;
		const double* GetDouble() const;
		const std::string* GetString() const;
		const bool* GetBool() const;
		void* GetClass() const;
		std::string GetType() const;

		bool isBool() const;
		bool isInteger()const;
		bool isDouble() const;
		bool isString() const;
		bool isClass() const;

		void Set(const ValueReference& value);

		std::string toString() const;

		ValueReference operator+(ValueReference& right);
		//prefix
		ValueReference& operator++();
		//postfix
		ValueReference operator++(int);
		//prefix
		ValueReference& operator--();
		//postfix
		ValueReference operator--(int);
		ValueReference operator-();
	};
}
