#pragma once
#include <variant>
#include <string>
#include <iostream>
#include <cassert>
#include <iostream> 
#include "utilities/Exceptions.h"

#define native_t std::variant<int*, double*, bool*, std::string*, void*> 

namespace interpreter {
	class ValueReference
	{
		native_t valueRefVariant;
		std::string type;
		ValueReference(int value);
		ValueReference(double value);
		ValueReference(std::string value);
		ValueReference(bool value);

	public:
		ValueReference();
		ValueReference(int* value);
		ValueReference(double* value);
		ValueReference(std::string* value);
		ValueReference(bool* value);
		ValueReference(void* value, std::string type = "class");
		ValueReference(ValueReference& value);
		//~ValueReference();


		int* GetInteger() const;
		double* GetDouble() const;
		std::string* GetString() const;
		bool* GetBool() const;
		void* GetClass() const;
		std::string GetType() const;

		bool isBool() const;
		bool isInteger()const;
		bool isDouble() const;
		bool isString() const;
		bool isClass() const;

		//void Set(ValueReference& value);

		std::string toString() const;

		ValueReference operator+(ValueReference& right);
		ValueReference operator-(ValueReference& right);
		ValueReference operator*(ValueReference& right);
		ValueReference operator/(ValueReference& right);
		ValueReference operator%(ValueReference& right);

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
