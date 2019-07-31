#include "utilities/ValueReference.h"

using namespace interpreter;

ValueReference::ValueReference() {
	valueRefVariant = (void*)nullptr;
}

ValueReference::ValueReference(int* value) {
	valueRefVariant = value;
}

ValueReference::ValueReference(int value) {
	valueRefVariant = new int(value);
}

ValueReference::ValueReference(double* value) {
	valueRefVariant = value;
}

ValueReference::ValueReference(double value) {
	valueRefVariant = new double(value);
}

ValueReference::ValueReference(std::string* value) {
	valueRefVariant = value;
}

ValueReference::ValueReference(std::string value) {
	valueRefVariant = new std::string(value);
}

ValueReference::ValueReference(bool* value) {
	valueRefVariant = value;
}

ValueReference::ValueReference(bool value) {
	valueRefVariant = new bool(value);
}

ValueReference::ValueReference(void* value, std::string type) {
	valueRefVariant = value;
	this->type = type;
}

ValueReference::ValueReference(const ValueReference& v) {
	valueRefVariant = v.valueRefVariant;
}


 int* ValueReference::GetInteger() const {
	return std::get<int*>(valueRefVariant);
}

double* ValueReference::GetDouble() const {
	return std::get<double*>(valueRefVariant);
}

std::string* ValueReference::GetString() const {
	return std::get<std::string*>(valueRefVariant);
}

bool* ValueReference::GetBool() const {
	return std::get<bool*>(valueRefVariant);
}

void* ValueReference::GetClass() const {
	return std::get<void*>(valueRefVariant);
}

std::string ValueReference::GetType() const {
	return type;
}

bool ValueReference::isBool() const {
	return std::holds_alternative<bool*>(valueRefVariant);
}

bool ValueReference::isInteger() const {
	return std::holds_alternative<int*>(valueRefVariant);
}

bool ValueReference::isDouble() const {
	return std::holds_alternative<double*>(valueRefVariant);
}

bool ValueReference::isString() const {
	return std::holds_alternative<std::string*>(valueRefVariant);
}

bool ValueReference::isClass() const {
	if (type.empty()) return false;
	return true;
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
void ValueReference::Set(ValueReference& v) {
	if (v.isBool())
		(*GetBool()) = (*v.GetBool());
	else if (v.isDouble())
		(*GetDouble()) = (*v.GetDouble());
	else if (v.isInteger())
		(*GetInteger()) = (*v.GetInteger());
	else if (v.isString())
		(*GetString()) = (*v.GetString());
}

void ValueReference::Set(int i) {
	(*GetInteger()) = i;
}

void ValueReference::Set(double d) {
	(*GetDouble()) = d;
}

void ValueReference::Set(bool b) {
	(*GetBool()) = b;
}

void ValueReference::Set(std::string s) {
	(*GetString()) = s;
}


ValueReference ValueReference::operator+(ValueReference& right) {
	ValueReference val;
	if (isInteger() && right.isInteger()) {
		val = *GetInteger() + *right.GetInteger();
	}
	else if (isString() && right.isString()) {
		val = *GetString() + *right.GetString();
	}
	else if (isDouble() && right.isDouble()) {
		val = *GetDouble() + *right.GetDouble();
	}
	else
		throw RuntimeErrorException("Non same types in operator+ in value reference ");
	return val;
}

ValueReference ValueReference::operator-(ValueReference& right) {
	ValueReference val;
	if (isInteger() && right.isInteger()) {
		val = *GetInteger() - *right.GetInteger();
	}
	else if (isDouble() && right.isDouble()) {
		val = *GetDouble() - *right.GetDouble();
	}
	else
		throw RuntimeErrorException("Non same types in operator- in value reference ");
	return val;
}

ValueReference ValueReference::operator*(ValueReference& right) {
	ValueReference val;
	if (isInteger() && right.isInteger()) {
		val = *GetInteger() * *right.GetInteger();
	}
	else if (isDouble() && right.isDouble()) {
		val = *GetDouble() * *right.GetDouble();
	}
	else
		throw RuntimeErrorException("Non same types in operator* in value reference ");
	return val;
}

ValueReference ValueReference::operator/(ValueReference& right) {
	ValueReference val;
	if (isInteger() && right.isInteger()) {
		val = *GetInteger() / *right.GetInteger();
	}
	else if (isDouble() && right.isDouble()) {
		val = *GetDouble() / *right.GetDouble();
	}
	else
		throw RuntimeErrorException("Non same types in operator/ in value reference ");
	return val;
}

ValueReference ValueReference::operator%(ValueReference& right) {
	ValueReference val;
	if (isInteger() && right.isInteger()) {
		val = *GetInteger() % *right.GetInteger();
	}
	else if (isDouble() && right.isDouble()) {
		val = (int)*GetDouble() % (int)*right.GetDouble();
	}
	else
		throw RuntimeErrorException("Non same types in operator% in value reference ");
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
		return (*GetInteger())++;
	}
	else if (isDouble()) {
		return (*GetDouble())++;
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
		(*GetDouble())--;
	}
	else
		throw RuntimeErrorException("Non numeric types in prefix decrement ");
	return *this;
}

//postfix
ValueReference ValueReference::operator--(int) {
	ValueReference val;
	if (isInteger()) {
		return (*GetInteger())--;
	}
	else if (isDouble()) {
		return (*GetDouble())--;
	}
	else
		throw RuntimeErrorException("Non numeric types in postfix decrement ");
}

ValueReference ValueReference::operator-() {
	ValueReference val;
	if (isInteger()) {
		val = -(*GetInteger());
	}
	else if (isDouble())
		val = - (*GetDouble());
	else
		throw RuntimeErrorException("Non numeric type in uminus ");
	return val;
}
