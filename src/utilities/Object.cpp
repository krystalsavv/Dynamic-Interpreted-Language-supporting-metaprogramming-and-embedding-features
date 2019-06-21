#include "utilities/Object.h"

Object::Object(const Value& key, const Value& value) {
	symbols[key] = value;
}

const Value& Object::GetValue(const Value& key) const {
	auto value = symbols.find(key); 
	if (value != symbols.end())
		return value->second;
	else {
		assert(false);
	}		
}

void Object::Set(const Value& key, const Value& value) {
	symbols[key] = value;
}

bool Object::ContainsKey(const Value& key) const {
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

std::ostream& operator << (std::ostream& out, const Object& obj) {
	out << "\n{" << std::endl;
	for (auto& [key, value] : obj.symbols) {
		out << "key: " << key << "\tvalue: " << value;
		out << std::endl;
	}
	out << "}" << std::endl;
	return out; 
}

