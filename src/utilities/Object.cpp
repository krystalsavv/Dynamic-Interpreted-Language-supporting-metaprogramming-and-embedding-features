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

