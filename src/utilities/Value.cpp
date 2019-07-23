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

Value::Value(double value) {
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

Value::Value(ValueReference* value) {
	variant = value;
}

Value::Value(ValueReference& value) {
	variant = &value;
}

Value::Value(const Value& value) {
	Set(value);
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

ValueReference* Value::GetValueReference() const {
	return std::get<ValueReference*>(variant);
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

bool Value::isValueReference() const {
	return std::holds_alternative<ValueReference*>(variant);
}


bool Value::toBool() const {
	if (isNumber()) {
		if (GetNumberValue() > 0)
			return true;
		return false;
	}
	else if (isString()) {
		if (GetStringValue().empty())
			return false;
		return true;
	}
	else if (isObject()) {
		if (!GetObjectValue())
			return false;
		return true;
	}
	else if (isBool()) {
		return GetBoolValue();
	}
	else if (isValueReference() && GetValueReference()) {
		if (GetValueReference()->isBool())
			return *GetValueReference()->GetBool();
		else if (GetValueReference()->isDouble()) {
			if (*GetValueReference()->GetDouble() > 0)
				return true;
			return false;
		}
		else if (GetValueReference()->isInteger()) {
			if (*GetValueReference()->GetInteger() > 0)
				return true;
			return false;
		}
		else if (GetValueReference()->isString()) {
			if (GetValueReference()->GetString()->empty())
				return false;
			return true;
		}
		else
			return true;
	}
	else if (isUndefined()) {
		throw RuntimeErrorException("Undefined to bool");
	}
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
	else if (isObject() && GetObjectValue())
		return GetObjectValue()->toString();
	else if (isObject() && !GetObjectValue())
		return "nil";
	else if (isValueReference() && GetValueReference()) {
		std::string s = "value reference: ";
		if (GetValueReference()->isBool())
			return (s + std::to_string(*GetValueReference()->GetBool()));
		else if (GetValueReference()->isDouble())
			return (s + std::to_string(*GetValueReference()->GetDouble()));
		else if (GetValueReference()->isInteger())
			return (s + std::to_string(*GetValueReference()->GetInteger()));
		else if (GetValueReference()->isString())
			return (s + *GetValueReference()->GetString());
		else
			return (s + GetValueReference()->GetType());
	}
	else {
		return "undefined";
	}
}

/****************************overloads***********************************/
//arithmetics
Value Value::operator+(Value& right) {
	Value val;
	if (isNumber() && right.isNumber())
		val = GetNumberValue() + right.GetNumberValue();
	else if (isString() && right.isString())
		val = GetStringValue() + right.GetStringValue();
	else if (isNumber() && right.isString())
		val = std::to_string(GetNumberValue()) + right.GetStringValue();
	else if (isString() && right.isNumber())
		val = GetStringValue() + std::to_string(right.GetNumberValue());
	else if (isValueReference() && GetValueReference() && right.isValueReference() && right.GetValueReference())
		val = *GetValueReference() + *right.GetValueReference();
	else {
		return Undefined();
	}
	return val;
}

Value Value::operator-(Value& right) {
	Value val;
	if (isNumber() && right.isNumber())
		val = GetNumberValue() - right.GetNumberValue();
	else if (isValueReference() && GetValueReference() && right.isValueReference() && right.GetValueReference()) {
		if (GetValueReference()->isInteger() && right.GetValueReference()->isInteger())
			val = (double)*GetValueReference()->GetInteger() - (double)*right.GetValueReference()->GetInteger();
		else if (GetValueReference()->isDouble() && right.GetValueReference()->isDouble())
			val = *GetValueReference()->GetDouble() - *right.GetValueReference()->GetDouble();
	}
	else
		throw RuntimeErrorException("Non numeric types in subtraction ");
	return val;
}


Value Value::operator*(Value& right) {
	Value val;
	if (isNumber() && right.isNumber())
		val = GetNumberValue() * right.GetNumberValue();
	else if (isValueReference() && GetValueReference() && right.isValueReference() && right.GetValueReference()) {
		if (GetValueReference()->isInteger() && right.GetValueReference()->isInteger())
			val = (double)*GetValueReference()->GetInteger() * (double)*right.GetValueReference()->GetInteger();
		else if (GetValueReference()->isDouble() && right.GetValueReference()->isDouble())
			val = *GetValueReference()->GetDouble() * *right.GetValueReference()->GetDouble();
	}
	else
		throw RuntimeErrorException("Non numeric types in multiplication ");
	return val;
}

Value Value::operator/(Value& right) {
	Value val;
	if (isNumber() && right.isNumber())
		val = GetNumberValue() / right.GetNumberValue();
	else if (isValueReference() && GetValueReference() && right.isValueReference() && right.GetValueReference()) {
		if (GetValueReference()->isInteger() && right.GetValueReference()->isInteger())
			val = (double)(*GetValueReference()->GetInteger() / *right.GetValueReference()->GetInteger());
		else if (GetValueReference()->isDouble() && right.GetValueReference()->isDouble())
			val = *GetValueReference()->GetDouble() / *right.GetValueReference()->GetDouble();
	}
	else
		throw RuntimeErrorException("Non numeric types in division ");
	return val;
}

Value Value::operator%(Value& right) {
	Value val;
	if (isNumber() && right.isNumber())
		val = (double)((int)GetNumberValue() % (int)right.GetNumberValue());
	else if (isValueReference() && GetValueReference() && right.isValueReference() && right.GetValueReference()) {
		if (GetValueReference()->isInteger() && right.GetValueReference()->isInteger())
			val = (double)(*GetValueReference()->GetInteger() % *right.GetValueReference()->GetInteger());
		else if (GetValueReference()->isDouble() && right.GetValueReference()->isDouble())
			val = (double)((int)*GetValueReference()->GetDouble() % (int)*right.GetValueReference()->GetDouble());
	}
	else
		throw RuntimeErrorException("Non numeric types in mod ");
	return val;
}

//prefix
Value& Value::operator++() {
	if (isNumber())
		Set(GetNumberValue() + 1);
	else if (isValueReference() && GetValueReference())
		++*GetValueReference();
	else
		throw RuntimeErrorException("Non numeric types in prefix increment ");
	return *this;
}

//postfix
Value Value::operator++(int) {
	Value val;
	if (isNumber()) {
		val = GetNumberValue();
		Set(GetNumberValue() + 1);
	}
	else
		throw RuntimeErrorException("Non numeric types in postfix increment ");
	return val;
}

//prefix
Value& Value::Value::operator--() {
	if (isNumber()) 
		Set(GetNumberValue() - 1);
	else
		throw RuntimeErrorException("Non numeric types in prefix decrement ");
	return *this;
}

//postfix
Value Value::operator--(int) {
	Value val;
	if (isNumber()) {
		val = GetNumberValue();
		Set(GetNumberValue() - 1);
	}
	else
		throw RuntimeErrorException("Non numeric types in postfix decrement ");
	return val;
}

Value Value::Value::operator-() {
	Value val;
	if (isNumber())
		val = -(GetNumberValue());
	else
		throw RuntimeErrorException("Non numeric type in uminus ");
	return val;
}


//equals
Value Value::operator>(Value& right) {
	Value val;
	if (isNumber() && right.isNumber())
		val = (GetNumberValue() > right.GetNumberValue());
	else if (isValueReference() && GetValueReference() && right.isValueReference() && right.GetValueReference()) {
		if (GetValueReference()->isInteger() && right.GetValueReference()->isInteger())
			val = (double)(*GetValueReference()->GetInteger() > *right.GetValueReference()->GetInteger());
		else if (GetValueReference()->isDouble() && right.GetValueReference()->isDouble())
			val = *GetValueReference()->GetDouble() > *right.GetValueReference()->GetDouble();
	}
	else
		throw RuntimeErrorException("Non numeric types in operator greater than (>)");
	return val;
}

Value Value::operator>=(Value& right) {
	Value val;
	if (isNumber() && right.isNumber())
		val = (GetNumberValue() >= right.GetNumberValue());
	else if (isValueReference() && GetValueReference() && right.isValueReference() && right.GetValueReference()) {
		if (GetValueReference()->isInteger() && right.GetValueReference()->isInteger())
			val = (double)(*GetValueReference()->GetInteger() >= *right.GetValueReference()->GetInteger());
		else if (GetValueReference()->isDouble() && right.GetValueReference()->isDouble())
			val = *GetValueReference()->GetDouble() >= *right.GetValueReference()->GetDouble();
	}
	else
		throw RuntimeErrorException("Non numeric types in operator greater than equal (>=)");
	return val;
}

Value Value::operator<(Value& right) {
	Value val;
	if (isNumber() && right.isNumber())
		val = (GetNumberValue() < right.GetNumberValue());
	else if (isValueReference() && GetValueReference() && right.isValueReference() && right.GetValueReference()) {
		if (GetValueReference()->isInteger() && right.GetValueReference()->isInteger())
			val = (double)(*GetValueReference()->GetInteger() < *right.GetValueReference()->GetInteger());
		else if (GetValueReference()->isDouble() && right.GetValueReference()->isDouble())
			val = *GetValueReference()->GetDouble() < *right.GetValueReference()->GetDouble();
	}
	else
		throw RuntimeErrorException("Non numeric types in operator less than (<)");
	return val;
}

Value Value::operator<=(Value& right) {
	Value val;
	if (isNumber() && right.isNumber())
		val = (GetNumberValue() <= right.GetNumberValue());
	else if (isValueReference() && GetValueReference() && right.isValueReference() && right.GetValueReference()) {
		if (GetValueReference()->isInteger() && right.GetValueReference()->isInteger())
			val = (double)(*GetValueReference()->GetInteger() <= *right.GetValueReference()->GetInteger());
		else if (GetValueReference()->isDouble() && right.GetValueReference()->isDouble())
			val = *GetValueReference()->GetDouble() <= *right.GetValueReference()->GetDouble();
	}
	else
		throw RuntimeErrorException("Non numeric types in operator less than equal (<=)");
	return val;
}


Value Value::operator==(Value& right) {
	Value val;
	if (isNumber() && right.isNumber())
		val = (GetNumberValue() == right.GetNumberValue());
	else if (isString() && right.isString())
		val = (GetStringValue() == right.GetStringValue());
	else if (isBool() && right.isBool())
		val = (GetBoolValue() == right.GetBoolValue());
	else if (isObject() && right.isObject()) 
		val = GetObjectValue() == right.GetObjectValue();
	else if (isValueReference() && GetValueReference() && right.isValueReference() && right.GetValueReference()) {
		if (GetValueReference()->isBool() && right.GetValueReference()->isBool())
			val = *GetValueReference()->GetBool() == *right.GetValueReference()->GetBool();
		else if (GetValueReference()->isInteger() && right.GetValueReference()->isInteger())
			val = *GetValueReference()->GetInteger() == *right.GetValueReference()->GetInteger();
		else if (GetValueReference()->isDouble() && right.GetValueReference()->isDouble())
			val = *GetValueReference()->GetDouble() == *right.GetValueReference()->GetDouble();
		else if (GetValueReference()->isString() && right.GetValueReference()->isString())
			val = *GetValueReference()->GetString() == *right.GetValueReference()->GetString();
		else if (GetValueReference()->isClass() && right.GetValueReference()->isClass())
			val = GetValueReference()->GetClass() == right.GetValueReference()->GetClass();
	}
	else
		//throw RuntimeErrorException("Invalid types in operator equal (==)");
		val = false;
	return val;
}

Value Value::operator!=(Value& right) {
	Value val;
	if (isNumber() && right.isNumber())
		val = (GetNumberValue() != right.GetNumberValue());
	else if (isString() && right.isString())
		val = (GetStringValue() != right.GetStringValue());
	else if (isBool() && right.isBool())
		val = (GetBoolValue() != right.GetBoolValue());
	else if (isObject() && right.isObject()) {
		val = GetObjectValue() != right.GetObjectValue();
	}
	else if (isValueReference() && GetValueReference() && right.isValueReference() && right.GetValueReference()) {
		if (GetValueReference()->isBool() && right.GetValueReference()->isBool())
			val = *GetValueReference()->GetBool() != *right.GetValueReference()->GetBool();
		else if (GetValueReference()->isInteger() && right.GetValueReference()->isInteger())
			val = *GetValueReference()->GetInteger() != *right.GetValueReference()->GetInteger();
		else if (GetValueReference()->isDouble() && right.GetValueReference()->isDouble())
			val = *GetValueReference()->GetDouble() != *right.GetValueReference()->GetDouble();
		else if (GetValueReference()->isString() && right.GetValueReference()->isString())
			val = *GetValueReference()->GetString() != *right.GetValueReference()->GetString();
		else if (GetValueReference()->isClass() && right.GetValueReference()->isClass())
			val = GetValueReference()->GetClass() != right.GetValueReference()->GetClass();
	}
	else
		//throw RuntimeErrorException("Invalid types in operator not equal (!=)");
		return true;
	return val;
}
//for consts (unordered map comparison)
bool Value::operator==(const Value& right) const {
	bool b{};
	if (isNumber() && right.isNumber())
		b = (GetNumberValue() == right.GetNumberValue());
	else if (isString() && right.isString())
		b = (GetStringValue() == right.GetStringValue());
	else if (isBool() && right.isBool())
		b = (GetBoolValue() == right.GetBoolValue());
	else if (isObject() && right.isObject()) {
			b = GetObjectValue() == right.GetObjectValue();
	}
	else if (isValueReference() && GetValueReference() && right.isValueReference() && right.GetValueReference()) {
		if (GetValueReference()->isBool() && right.GetValueReference()->isBool())
			b = *GetValueReference()->GetBool() == *right.GetValueReference()->GetBool();
		else if (GetValueReference()->isInteger() && right.GetValueReference()->isInteger())
			b = *GetValueReference()->GetInteger() == *right.GetValueReference()->GetInteger();
		else if (GetValueReference()->isDouble() && right.GetValueReference()->isDouble())
			b = *GetValueReference()->GetDouble() == *right.GetValueReference()->GetDouble();
		else if (GetValueReference()->isString() && right.GetValueReference()->isString())
			b = *GetValueReference()->GetString() == *right.GetValueReference()->GetString();
		else if (GetValueReference()->isClass() && right.GetValueReference()->isClass())
			b = GetValueReference()->GetClass() == right.GetValueReference()->GetClass();
	}
	else
		b = false;
	return b;
}

//for consts (unordered map comparison)
bool Value::operator!=(const Value& right) const {
	bool b{};
	if (isNumber() && right.isNumber())
		b = (GetNumberValue() != right.GetNumberValue());
	else if (isString() && right.isString())
		b = (GetStringValue() != right.GetStringValue());
	else if (isBool() && right.isBool())
		b = (GetBoolValue() != right.GetBoolValue());
	else if (isObject() && right.isObject()) {
		b = GetObjectValue() != right.GetObjectValue();
	}
	else if (isValueReference() && GetValueReference() && right.isValueReference() && right.GetValueReference()) {
		if (GetValueReference()->isBool() && right.GetValueReference()->isBool())
			b = *GetValueReference()->GetBool() != *right.GetValueReference()->GetBool();
		else if (GetValueReference()->isInteger() && right.GetValueReference()->isInteger())
			b = *GetValueReference()->GetInteger() != *right.GetValueReference()->GetInteger();
		else if (GetValueReference()->isDouble() && right.GetValueReference()->isDouble())
			b = *GetValueReference()->GetDouble() != *right.GetValueReference()->GetDouble();
		else if (GetValueReference()->isString() && right.GetValueReference()->isString())
			b = *GetValueReference()->GetString() != *right.GetValueReference()->GetString();
		else if (GetValueReference()->isClass() && right.GetValueReference()->isClass())
			b = GetValueReference()->GetClass() != right.GetValueReference()->GetClass();
	}
	else
		b = true;
	return b;
}

//logical
Value Value::operator&&(Value& right) {
	return (toBool() && right.toBool());
}

Value Value::operator||(Value& right) {
	return (toBool() || right.toBool());
}

Value Value::operator!() {
	return !(toBool());
}

std::ostream& interpreter::operator<< (std::ostream& out, const Value& value) {
	return out << value.toString();
}