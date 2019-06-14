#include "utilities/Object.h"


Object::Object(std::string key, bool value) {
	Value* val = new Value(value);
	symbols[key] = val;
}

Object::Object(std::string key, double value) {
	Value* val = new Value(value);
	symbols[key] = val;
}

Object::Object(std::string key, std::string value) {
	Value* val = new Value(value);
	symbols[key] = val;
}

Object::Object(std::string key, Object* value) {
	Value* val = new Value(value);
	symbols[key] = val;
}

void Object::Set(std::string key, bool value) {
	symbols[key]->Set(value);
}

void Object::Set(std::string key, double value) {
	symbols[key]->Set(value);
}

void Object::Set(std::string key, std::string value) {
	symbols[key]->Set(value);
}

void Object::Set(std::string key, Object* value) {
	symbols[key]->Set(value);
}


Value* Object::GetValue(std::string key) {
	return symbols[key];
}


bool Object::ContainsKey(std::string key) {
	if (symbols.find(key) != symbols.end())
		return true;
	else
		return false;
}


void Object::PrintMap() {
	for (auto& [key, value] : symbols) {
		std::cout << "key: " << key << "value: " << value;
	}
}

