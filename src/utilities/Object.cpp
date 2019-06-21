#include "utilities/Object.h"

Object::Object(const Value& key, const Value& value) {
	symbols[key] = value;
}

Value& Object::GetValue(const Value& key) {
	return symbols[key];
}

bool Object::ContainsKey(const Value& key) const{
	if (symbols.find(key) != symbols.end())
		return true;
	else
		return false;
}

//overloads
Value Object::operator==(Object* obj) {
	return (this == obj);
}

Value  Object::operator!=(Object* obj) {
	return (this != obj);
}

std::ostream& operator << (std::ostream& out, const Object& o) {
	out << "{" << std::endl;
	for (auto& [key, value] : o.symbols) {
		out << "key: " << key << "value: " << value;
		out << std::endl;
	}
	out << "}" << std::endl;
	return out; 
}