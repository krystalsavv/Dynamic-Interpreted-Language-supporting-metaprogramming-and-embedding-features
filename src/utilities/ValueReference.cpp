#include "utilities/ValueReference.h"

using namespace interpreter;

ValueReference::ValueReference() {
	variant = (void*)nullptr;
}

ValueReference::ValueReference(int* value) {
	variant = value;
}

ValueReference::ValueReference(int value) {
	variant = (int*)&value;
}

ValueReference::ValueReference(double* value) {
	variant = value;
}

ValueReference::ValueReference(double value) {
	variant = (double*)&value;
}

ValueReference::ValueReference(std::string* value) {
	variant = value;
}

ValueReference::ValueReference(std::string value) {
	variant = (std::string*)&value;
}

ValueReference::ValueReference(bool* value) {
	variant = value;
}

ValueReference::ValueReference(bool value) {
	variant = (bool*)&value;
}

ValueReference::ValueReference(void* value, std::string type) {
	variant = value;
	this->type = type;
}

ValueReference::ValueReference(const ValueReference& value) {
	Set(value);
}

 int* ValueReference::GetInteger() const {
	return std::get<int*>(variant);
}

double* ValueReference::GetDouble() const {
	return std::get<double*>(variant);
}

std::string* ValueReference::GetString() const {
	return std::get<std::string*>(variant);
}

bool* ValueReference::GetBool() const {
	return std::get<bool*>(variant);
}

void* ValueReference::GetClass() const {
	return std::get<void*>(variant);
}

std::string ValueReference::GetType() const {
	return type;
}

bool ValueReference::isBool() const {
	return std::holds_alternative<bool*>(variant);
}

bool ValueReference::isInteger() const {
	return std::holds_alternative<int*>(variant);
}

bool ValueReference::isDouble() const {
	return std::holds_alternative<double*>(variant);
}

bool ValueReference::isString() const {
	return std::holds_alternative<std::string*>(variant);
}

bool ValueReference::isClass() const {
	if (type.empty()) return false;
	return true;
}

void ValueReference::Set(const ValueReference& value) {
	variant = value.variant;
}

std::string ValueReference::toString() const {
	if (isBool())
		return std::to_string(*GetBool());
	else if (isInteger())
		return std::to_string(*GetInteger());
	else if (isString())
		return *GetString();
	else if (isDouble())
		return std::to_string(*GetDouble());
	else
		return type;
}


ValueReference ValueReference::operator+(ValueReference& right) {
	ValueReference val;
	if (isInteger() && right.isInteger()) {
		int i = *GetInteger() + *right.GetInteger();
		val = &i;
	}
	else if (isString() && right.isString()) {
		std::string s = *GetString() + *right.GetString();
		val = &s;
	}
	else if (isDouble() && right.isDouble()) {
		double d = *GetDouble() + *right.GetDouble();
		val = &d;
	}
	else
		throw RuntimeErrorException("Non same types in operator + in value reference ");
	return val;
}

//prefix
ValueReference& ValueReference::operator++() {
	if (isInteger())
		(*GetInteger())++;
	else if (isDouble()) 
		(*GetDouble())++;
	else
		throw RuntimeErrorException("Non numeric types in prefix increment ");
	return *this;
}

//postfix
ValueReference ValueReference::operator++(int) {
	if (isInteger()) {
		int x = (*GetInteger())++;
		std::cout << x << std::endl;
		return x;
	}
	else if (isDouble()) {
		return 5;
	}
	else
		throw RuntimeErrorException("Non numeric types in postfix increment ");
}

//prefix
ValueReference& ValueReference::operator--() {
	if (isInteger()) {
		(*GetInteger())--;
	}
	else if (isDouble()) {
		double d = *GetDouble() - 1;
		Set(&d);
	}
	else
		throw RuntimeErrorException("Non numeric types in prefix decrement ");
	return *this;
}

//postfix
ValueReference ValueReference::operator--(int) {
	ValueReference val;
	if (isInteger()) {
		val = (int*)GetInteger();
		int i = *GetInteger() - 1;
		Set(&i);
	}
	else if (isDouble()) {
		val = (double*)GetDouble();
		double d = *GetDouble() - 1;
		Set(&d);
	}
	else
		throw RuntimeErrorException("Non numeric types in postfix increment ");
	return val;
}

ValueReference ValueReference::operator-() {
	ValueReference val;
	if (isInteger()) {
		int i = *GetInteger() - 1;
	}
	else if (isDouble())
		val = -(*GetDouble());
	else
		throw RuntimeErrorException("Non numeric type in uminus ");
	return val;
}
