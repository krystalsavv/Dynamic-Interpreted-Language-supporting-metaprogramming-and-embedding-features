#include "utilities/ValueReference.h"

using namespace interpreter;

ValueReference::ValueReference() {
	variant = (void*)nullptr;
}

ValueReference::ValueReference(int* value) {
	variant = value;
}

ValueReference::ValueReference(int value) {
	variant = &value;
}

ValueReference::ValueReference(double* value) {
	variant = value;
}

ValueReference::ValueReference(double value) {
	variant = &value;
}

ValueReference::ValueReference(std::string* value) {
	variant = value;
}

ValueReference::ValueReference(std::string value) {
	variant = &value;
}

ValueReference::ValueReference(bool* value) {
	variant = value;
}

ValueReference::ValueReference(bool value) {
	variant = &value;
}

ValueReference::ValueReference(void* value, std::string type) {
	variant = value;
	this->type = type;
}

ValueReference::ValueReference(const ValueReference& value) {
	Set(value);
}

const int* ValueReference::GetInteger() const {
	return std::get<int*>(variant);
}

const double* ValueReference::GetDouble() const {
	return std::get<double*>(variant);
}

const std::string* ValueReference::GetString() const {
	return std::get<std::string*>(variant);
}

const bool* ValueReference::GetBool() const {
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
	if (isInteger() && right.isInteger())
		val = *GetInteger() + *right.GetInteger();
	else if (isString() && right.isString())
		val = *GetString() + *right.GetString();
	else if (isDouble() && right.isDouble())
		val = *GetDouble() + *right.GetDouble();
	else
		throw RuntimeErrorException("Non same types in operator + in value reference ");
	return val;
}

//prefix
ValueReference& ValueReference::operator++() {
	if (isInteger())
		Set( *GetInteger() + 1);
	else if (isDouble())
		Set(*GetDouble() + 1);
	else
		throw RuntimeErrorException("Non numeric types in prefix increment ");
	return *this;
}

//postfix
ValueReference ValueReference::operator++(int) {
	ValueReference val;
	if (isInteger()) {
		val = (int*)GetInteger();
		Set( *GetInteger() + 1);
	}
	else if (isDouble()) {
		val = (double*)GetDouble();
		Set(*GetDouble() + 1);
	}
	else
		throw RuntimeErrorException("Non numeric types in postfix increment ");
	return val;
}

//prefix
ValueReference& ValueReference::operator--() {
	if (isInteger())
		Set(*GetInteger() - 1);
	else if (isDouble())
		Set(*GetDouble() - 1);
	else
		throw RuntimeErrorException("Non numeric types in prefix decrement ");
	return *this;
}

//postfix
ValueReference ValueReference::operator--(int) {
	ValueReference val;
	if (isInteger()) {
		val = (int*)GetInteger();
		Set(*GetInteger() - 1);
	}
	else if (isDouble()) {
		val = (double*)GetDouble();
		Set(*GetDouble() - 1);
	}
	else
		throw RuntimeErrorException("Non numeric types in postfix increment ");
	return val;
}

ValueReference ValueReference::operator-() {
	ValueReference val;
	if (isInteger())
		val =  -(*GetInteger());
	if (isDouble())
		val = -(*GetDouble());
	else
		throw RuntimeErrorException("Non numeric type in uminus ");
	return val;
}
