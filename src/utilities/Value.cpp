#include "utilities/Value.h"
#include "utilities/Object.h"


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

Value::Value( Object* value) {
	variant = value;
}


//getters
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

bool Value::toBool() {
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
	// TODO: libfunc
}

void Value::PrintValue() const{
	if (this->isBool())
		std::cout << this->GetBoolValue();
	else if (this->isNumber())
		std::cout << this->GetNumberValue();
	else if (this->isString())
		std::cout << this->GetStringValue();
	else if (this->isObject())
		this->GetObjectValue()->PrintMap();
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
		// TODO: error in different types
		assert(false);
	}
	return val;
}

Value Value::operator-(Value& right) {
	Value val;
	if (this->isNumber() && right.isNumber())
		val = this->GetNumberValue() - right.GetNumberValue();
	else
		// TODO: error in different types
		assert(false);
	return val;
}

Value Value::operator*(Value& right) {
	Value val;
	if (this->isNumber() && right.isNumber())
		val = this->GetNumberValue() * right.GetNumberValue();
	else
		// TODO: error in different types
		assert(false);
	return val;
}

Value Value::operator/(Value& right) {
	Value val;
	if (this->isNumber() && right.isNumber())
		val = this->GetNumberValue() / right.GetNumberValue();
	else
		// TODO: error in different types
		assert(false);
	return val;
}

Value Value::operator%(Value& right) {
	Value val;
	if (this->isNumber() && right.isNumber())
		val = (double)((int)this->GetNumberValue() % (int)right.GetNumberValue());
	else
		// TODO: error in different types
		assert(false);
	return val;
}

//prefix
Value& Value::operator++() {
	Value val;
	if (this->isNumber()) 
		this->Set(this->GetNumberValue() + 1);
	else
		// TODO: error in different types
		assert(false);
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
		// TODO: error in different types
		assert(false);
	return val;
}

//prefix
Value& Value::Value::operator--() {
	if (this->isNumber()) 
		this->Set(this->GetNumberValue() - 1);
	else
		// TODO: error in different types
		assert(false);
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
		// TODO: error in different types
		assert(false);
	return val;
}

Value Value::Value::operator-() {
	Value val;
	if (this->isNumber())
		val = -(this->GetNumberValue());
	else
		// TODO: error in different types
		assert(false);
	return val;
}


//equals
Value Value::operator>(Value& right) {
	Value val;
	if (this->isNumber() && right.isNumber())
		val = (this->GetNumberValue() > right.GetNumberValue());
	else
		// TODO: error in different types
		val = false;
	return val;
}

Value Value::operator>=(Value& right) {
	Value val;
	if (this->isNumber() && right.isNumber())
		val = (this->GetNumberValue() >= right.GetNumberValue());
	else
		// TODO: error in different types
		val = false;
	return val;
}

Value Value::operator<(Value& right) {
	Value val;
	if (this->isNumber() && right.isNumber())
		val = (this->GetNumberValue() < right.GetNumberValue());
	else
		// TODO: error in different types
		val = false;
	return val;
}

Value Value::operator<=(Value& right) {
	Value val;
	if (this->isNumber() && right.isNumber())
		val = (this->GetNumberValue() <= right.GetNumberValue());
	else
		// TODO: error in different types
		val = false;
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
	// TODO: NO OVERLOAD FOR OBJECT, STRANGER THINGS
	else if (this->isObject() && right.isObject()) {
		if (this->GetObjectValue() == nullptr || right.GetObjectValue() == nullptr)
			val = false;
		else
			val = this->GetObjectValue() == right.GetObjectValue();
	}
	else
		// TODO: error in different types
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
	// TODO: NO OVERLOAD FOR OBJECT, STRANGER THINGS
	else if (this->isObject() && right.isObject()) {
		if (this->GetObjectValue() == NULL || right.GetObjectValue() == NULL)
			val = true;
		else
			val = this->GetObjectValue() != right.GetObjectValue();
	}
	else
		// TODO: error in different types
		val = true;
	return val;
}
//for consts (unordered map comparison)
//Value Value::operator==(const Value& right) const {
//	Value val;
//	if (this->isNumber() && right.isNumber())
//		val = (this->GetNumberValue() == right.GetNumberValue());
//	else if (this->isString() && right.isString())
//		val = (this->GetStringValue() == right.GetStringValue());
//	else if (this->isBool() && right.isBool())
//		val = (this->GetBoolValue() == right.GetBoolValue());
//	// TODO: NO OVERLOAD FOR OBJECT, STRANGER THINGS
//	else if (this->isObject() && right.isObject()) {
//		if (this->GetObjectValue() == nullptr || right.GetObjectValue() == nullptr)
//			val = false;
//		else
//			val = this->GetObjectValue() == right.GetObjectValue();
//	}
//	else
//		// TODO: error in different types
//		val = false;
//	return val;
//}
//for consts (unordered map comparison)
//Value Value::operator!=(const Value& right) const {
//	Value val;
//	if (this->isNumber() && right.isNumber())
//		val = (this->GetNumberValue() != right.GetNumberValue());
//	else if (this->isString() && right.isString())
//		val = (this->GetStringValue() != right.GetStringValue());
//	else if (this->isBool() && right.isBool())
//		val = (this->GetBoolValue() != right.GetBoolValue());
//	// TODO: NO OVERLOAD FOR OBJECT, STRANGER THINGS
//	else if (this->isObject() && right.isObject()) {
//		if (this->GetObjectValue() == NULL || right.GetObjectValue() == NULL)
//			val = true;
//		else
//			val = this->GetObjectValue() != right.GetObjectValue();
//	}
//	else
//		// TODO: error in different types
//		val = true;
//	return val;
//}

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
