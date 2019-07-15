#include "utilities/Value.h"
#include "utilities/Object.h"
#include <cassert>

using namespace interpreter;

Value undefined(Undefined());

Value::Value()
{
	variant = Undefined();
}

Value::Value(bool value) {
	variant = value;
}

Value::Value( double value) {
	variant = value;
}

Value::Value(std::string value) {
	variant = value;
}

Value::Value(const char * value) {
	variant = std::string(value);
}

Value::Value(Object* value) {
	variant = value;
}

Value::Value(Undefined value) {
	variant = value;
}

Value::Value(const Value& value) {
	variant = value.variant;
}


// Getters
const bool& Value::GetBoolValue() const {
	return std::get<bool>(variant);
}

const double& Value::GetNumberValue() const {
	return std::get<double>(variant);
}

const std::string& Value::GetStringValue() const {
	return std::get<std::string>(variant);
}

Object* Value::GetObjectValue() const {
	return std::get<Object*>(variant);
}


bool Value::isBool() const {
	return std::holds_alternative<bool>(variant);
}

bool Value::isNumber() const {
	return std::holds_alternative<double>(variant);
}

bool Value::isString() const {
	return std::holds_alternative<std::string>(variant);
}

bool Value::isObject() const {
	return std::holds_alternative<Object*>(variant);
}

bool Value::isUndefined() const {
	return std::holds_alternative<Undefined>(variant);
}


bool Value::toBool() const {
	if (this->isNumber()) {
		if (this->GetNumberValue() > 0)
			return true;
		else
			return false;
	}
	else if (this->isString()) {
		if (this->GetStringValue().empty())
			return false;
		else
			return true;
	}
	else if (this->isObject()) {
		if (this->GetObjectValue() == nullptr)
			return false;
		else
			return true;
	}
	else if (this->isBool()) {
		return this->GetBoolValue();
	}
	else if (isUndefined()) {
		throw RuntimeErrorException("Undefined to bool");
	}
	// TODO: libfunc
}


void Value::Set(const Value& value) {
	variant = value.variant;
}


std::string Value::toString() const {
	if (isBool())
		return std::to_string(GetBoolValue());
	else if (isNumber())
		return std::to_string(GetNumberValue());
	else if (isString())
		return GetStringValue();
	else if (isObject() && GetObjectValue() != nullptr)
		return GetObjectValue()->toString();
	else if (isObject() && GetObjectValue() == nullptr)
		return "null";
	else {
		return "undefined";
	}
}

/****************************overloads***********************************/
//arithmetics
Value Value::operator+(Value& right) {
	Value val;
	if (this->isNumber() && right.isNumber())
		val = this->GetNumberValue() + right.GetNumberValue();
	else if (this->isString() && right.isString())
		val = this->GetStringValue() + right.GetStringValue();
	else if (this->isNumber() && right.isString())
		val = std::to_string(this->GetNumberValue()) + right.GetStringValue();
	else if (this->isString() && right.isNumber())
		val = this->GetStringValue() + std::to_string(right.GetNumberValue());
	else {
		return Undefined();
	}
	return val;
}

Value Value::operator-(Value& right) {
	Value val;
	if (this->isNumber() && right.isNumber())
		val = this->GetNumberValue() - right.GetNumberValue();
	else
		throw RuntimeErrorException("Non numeric types in subtraction ");
	return val;
}


Value Value::operator*(Value& right) {
	Value val;
	if (this->isNumber() && right.isNumber())
		val = this->GetNumberValue() * right.GetNumberValue();
	else
		throw RuntimeErrorException("Non numeric types in multiplication ");
	return val;
}

Value Value::operator/(Value& right) {
	Value val;
	if (this->isNumber() && right.isNumber())
		val = this->GetNumberValue() / right.GetNumberValue();
	else
		throw RuntimeErrorException("Non numeric types in division ");
	return val;
}

Value Value::operator%(Value& right) {
	Value val;
	if (this->isNumber() && right.isNumber())
		val = (double)((int)this->GetNumberValue() % (int)right.GetNumberValue());
	else
		throw RuntimeErrorException("Non numeric types in mod ");
	return val;
}

//prefix
Value& Value::operator++() {
	Value val;
	if (this->isNumber()) 
		this->Set(this->GetNumberValue() + 1);
	else
		throw RuntimeErrorException("Non numeric types in prefix increment ");
	return *this;
}

//postfix
Value Value::operator++(int) {
	Value val;
	if (this->isNumber()) {
		val = this->GetNumberValue();
		this->Set(this->GetNumberValue() + 1);
	}
	else
		throw RuntimeErrorException("Non numeric types in postfix increment ");
	return val;
}

//prefix
Value& Value::Value::operator--() {
	if (this->isNumber()) 
		this->Set(this->GetNumberValue() - 1);
	else
		throw RuntimeErrorException("Non numeric types in prefix decrement ");
	return *this;
}

//postfix
Value Value::operator--(int) {
	Value val;
	if (this->isNumber()) {
		val = this->GetNumberValue();
		this->Set(this->GetNumberValue() - 1);
	}
	else
		throw RuntimeErrorException("Non numeric types in postfix decrement ");
	return val;
}

Value Value::Value::operator-() {
	Value val;
	if (this->isNumber())
		val = -(this->GetNumberValue());
	else
		throw RuntimeErrorException("Non numeric type in uminus ");
	return val;
}


//equals
Value Value::operator>(Value& right) {
	Value val;
	if (this->isNumber() && right.isNumber())
		val = (this->GetNumberValue() > right.GetNumberValue());
	else
		throw RuntimeErrorException("Non numeric types in operator greater than (>)");
	return val;
}

Value Value::operator>=(Value& right) {
	Value val;
	if (this->isNumber() && right.isNumber())
		val = (this->GetNumberValue() >= right.GetNumberValue());
	else
		throw RuntimeErrorException("Non numeric types in operator greater than equal (>=)");
	return val;
}

Value Value::operator<(Value& right) {
	Value val;
	if (this->isNumber() && right.isNumber())
		val = (this->GetNumberValue() < right.GetNumberValue());
	else
		throw RuntimeErrorException("Non numeric types in operator less than (<)");
	return val;
}

Value Value::operator<=(Value& right) {
	Value val;
	if (this->isNumber() && right.isNumber())
		val = (this->GetNumberValue() <= right.GetNumberValue());
	else
		throw RuntimeErrorException("Non numeric types in operator less than equal (<=)");
	return val;
}


Value Value::operator==(Value& right) {
	Value val;
	if (this->isNumber() && right.isNumber())
		val = (this->GetNumberValue() == right.GetNumberValue());
	else if (this->isString() && right.isString())
		val = (this->GetStringValue() == right.GetStringValue());
	else if (this->isBool() && right.isBool())
		val = (this->GetBoolValue() == right.GetBoolValue());
	else if (this->isObject() && right.isObject()) {
		val = this->GetObjectValue() == right.GetObjectValue();
	}
	else
		//throw RuntimeErrorException("Invalid types in operator equal (==)");
		val = false;
	return val;
}

Value Value::operator!=(Value& right) {
	Value val;
	if (this->isNumber() && right.isNumber())
		val = (this->GetNumberValue() != right.GetNumberValue());
	else if (this->isString() && right.isString())
		val = (this->GetStringValue() != right.GetStringValue());
	else if (this->isBool() && right.isBool())
		val = (this->GetBoolValue() != right.GetBoolValue());
	else if (this->isObject() && right.isObject()) {
		val = this->GetObjectValue() != right.GetObjectValue();
	}
	else
		//throw RuntimeErrorException("Invalid types in operator not equal (!=)");
		return true;
	return val;
}
//for consts (unordered map comparison)
bool Value::operator==(const Value& right) const {
	bool b;
	if (this->isNumber() && right.isNumber())
		b = (this->GetNumberValue() == right.GetNumberValue());
	else if (this->isString() && right.isString())
		b = (this->GetStringValue() == right.GetStringValue());
	else if (this->isBool() && right.isBool())
		b = (this->GetBoolValue() == right.GetBoolValue());
	else if (this->isObject() && right.isObject()) {
			b = this->GetObjectValue() == right.GetObjectValue();
	}
	else
		//throw RuntimeErrorException("Invalid types in operator equal (==)");
		b = false;
	return b;
}

//for consts (unordered map comparison)
bool Value::operator!=(const Value& right) const {
	bool b;
	if (this->isNumber() && right.isNumber())
		b = (this->GetNumberValue() != right.GetNumberValue());
	else if (this->isString() && right.isString())
		b = (this->GetStringValue() != right.GetStringValue());
	else if (this->isBool() && right.isBool())
		b = (this->GetBoolValue() != right.GetBoolValue());
	else if (this->isObject() && right.isObject()) {
		b = this->GetObjectValue() != right.GetObjectValue();
	}
	else
		//throw RuntimeErrorException("Invalid types in operator not equal (!=)");
		b = true;
	return b;
}

//logical
Value Value::operator&&(Value& right) {
	return (this->toBool() && right.toBool());
}

Value Value::operator||(Value& right) {
	return (this->toBool() || right.toBool());
}

Value Value::operator!() {
	return !(this->toBool());
}

std::ostream& interpreter::operator<< (std::ostream& out, const Value& value) {
	return out << value.toString();
}