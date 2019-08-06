#include "utilities/ValueReference.h"

using namespace interpreter;

ValueReference::ValueReference() {
	valueRefVariant = (void*)nullptr;
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

ValueReference::ValueReference(const ValueReference& v) {
	valueRefVariant = v.valueRefVariant;
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


bool ValueReference::isBool() const {
	return std::holds_alternative<bool*>(valueRefVariant);
}


bool ValueReference::isDouble() const {
	return std::holds_alternative<double*>(valueRefVariant);
}

bool ValueReference::isString() const {
	return std::holds_alternative<std::string*>(valueRefVariant);
}


std::string ValueReference::toString() const {
	if (isBool())
		return std::to_string(*GetBool());
	else if (isString())
		return *GetString();
	else if (isDouble())
		return std::to_string(*GetDouble());
}
void ValueReference::Set(ValueReference& v) {
	if (v.isBool())
		(*GetBool()) = (*v.GetBool());
	else if (v.isDouble())
		(*GetDouble()) = (*v.GetDouble());
	else if (v.isString())
		(*GetString()) = (*v.GetString());
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
	if (isString() && right.isString()) {
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
	 if (isDouble() && right.isDouble()) {
		val = *GetDouble() - *right.GetDouble();
	}
	else
		throw RuntimeErrorException("Non same types in operator- in value reference ");
	return val;
}

ValueReference ValueReference::operator*(ValueReference& right) {
	ValueReference val;
	 if (isDouble() && right.isDouble()) {
		val = *GetDouble() * *right.GetDouble();
	}
	else
		throw RuntimeErrorException("Non same types in operator* in value reference ");
	return val;
}

ValueReference ValueReference::operator/(ValueReference& right) {
	ValueReference val;
	 if (isDouble() && right.isDouble()) {
		val = *GetDouble() / *right.GetDouble();
	}
	else
		throw RuntimeErrorException("Non same types in operator/ in value reference ");
	return val;
}

ValueReference ValueReference::operator%(ValueReference& right) {
	ValueReference val;
	 if (isDouble() && right.isDouble()) {
		val = (double)((int)*GetDouble() % (int)*right.GetDouble());
	}
	else
		throw RuntimeErrorException("Non same types in operator% in value reference ");
	return val;
}

//prefix
ValueReference& ValueReference::operator++() {
	 if (isDouble()) 
		(*GetDouble())++;
	else
		throw RuntimeErrorException("Non numeric types in prefix increment ");
	return *this;
}

//postfix
ValueReference ValueReference::operator++(int) {
	 if (isDouble()) {
		return (*GetDouble())++;
	}
	else
		throw RuntimeErrorException("Non numeric types in postfix increment ");
}

//prefix
ValueReference& ValueReference::operator--() {
	 if (isDouble()) {
		(*GetDouble())--;
	}
	else
		throw RuntimeErrorException("Non numeric types in prefix decrement ");
	return *this;
}

//postfix
ValueReference ValueReference::operator--(int) {
	ValueReference val;
	 if (isDouble()) {
		return (*GetDouble())--;
	}
	else
		throw RuntimeErrorException("Non numeric types in postfix decrement ");
}

ValueReference ValueReference::operator-() {
	ValueReference val;
	 if (isDouble())
		val = - (*GetDouble());
	else
		throw RuntimeErrorException("Non numeric type in uminus ");
	return val;
}
