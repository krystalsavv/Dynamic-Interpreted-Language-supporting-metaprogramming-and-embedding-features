#include "utilities/Value.h"
#include "utilities/Object.h"

Value::Value( bool value) {
	variant = value;
}

Value::Value( double value) {
	variant = value;
}

Value::Value(std::string value) {
	variant = value;
}

Value::Value( Object* value) {
	variant = value;
}

//setters
void Value::Set(bool value) {
	variant = value;
}

void  Value::Set( double value) {
	variant = value;
}

void  Value::Set( std::string value) {
	variant = value;
}

void  Value::Set( Object* value) {
	variant = value;
}

//getters
bool& Value::GetBoolValue() {
	return std::get<bool>(variant);
}

double& Value::GetNumberValue() {
	return std::get<double>(variant);
}

std::string& Value::GetStringValue() {
	return std::get<std::string>(variant);
}

Object* Value::GetObjectValue() {
	return std::get<Object*>(variant);
}


bool Value::isBool() {
	return std::holds_alternative<bool>(variant);
}

bool Value::isNumber() {
	return std::holds_alternative<double>(variant);
}

bool Value::isString() {
	return std::holds_alternative<std::string>(variant);
}

bool Value::isObject() {
	return std::holds_alternative<Object*>(variant);
}

void Value::PrintValue() {
	if (this->isBool())
		std::cout << this->GetBoolValue();
	else if (this->isNumber())
		std::cout << this->GetNumberValue();
	else if (this->isString())
		std::cout << this->GetStringValue();
	else if (this->isObject())
		this->GetObjectValue()->PrintMap();
}