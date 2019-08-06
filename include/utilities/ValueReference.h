#pragma once
#include <variant>
#include <string>
#include <iostream>
#include <cassert>
#include <iostream> 
#include "utilities/Exceptions.h"

#define native_t std::variant<double*, bool*, std::string*, void*> 

namespace interpreter {
	class ValueReference
	{
		native_t valueRefVariant;
		ValueReference(double value);
		ValueReference(std::string value);
		ValueReference(bool value);

	public:
		ValueReference();
		ValueReference(double* value);
		ValueReference(std::string* value);
		ValueReference(bool* value);
		ValueReference(const ValueReference& v);


		double* GetDouble() const;
		std::string* GetString() const;
		bool* GetBool() const;

		bool isBool() const;
		bool isDouble() const;
		bool isString() const;

		void Set(ValueReference& v);
		void Set(double d);
		void Set(bool b);
		void Set(std::string s);

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
