#include "utilities/Object.h"


Object::Object(std::string key, bool value) {
	symbols[key] = value;
}

Object::Object(std::string key, double value) {
	symbols[key] = value;
}

Object::Object(std::string key, std::string value) {
	symbols[key] = value;
}

Object::Object(std::string key, const char* value) {
	symbols[key] = std::string(value);
}


Object::Object(std::string key, Object* value) {
	symbols[key] = value;
}


Value& Object::GetValue(std::string key) {
	return symbols[key];
}


bool Object::ContainsKey(std::string key) const{
	if (symbols.find(key) != symbols.end())
		return true;
	else
		return false;
}

void Object::PrintMap() const{
	for (auto& [key, value] : symbols) {
		std::cout << "key: " << key << "value: ";  
		value.PrintValue();
	}
}

//overloads
Value Object::operator==(Object* obj) {
	return (this == obj);
}

Value  Object::operator!=(Object* obj) {
	return (this != obj);
}