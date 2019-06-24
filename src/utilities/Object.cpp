#include "utilities/Object.h"

unsigned int Object::nestedCounterPrint = 0;

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

void Object::PrintTabs(std::ostream& out) {
	for (int i = 0; i < Object::nestedCounterPrint; i++) {
		out << "\t";
	}
}

//overloads
Value Object::operator==(Object* obj) {
	return (this == obj);
}

Value  Object::operator!=(Object* obj) {
	return (this != obj);
}

std::ostream& operator << (std::ostream& out, const Object& obj) {
	out << "{" << std::endl;
	Object::nestedCounterPrint++;
	int commaCounter = 0;
	for (auto& [key, value] : obj.symbols) {
		Object::PrintTabs(out);
		out << key << " : " << value ;
		if (commaCounter++ < obj.symbols.size()-1)
			out << ", ";
		out << std::endl;	
	}
	Object::nestedCounterPrint--;
	Object::PrintTabs(out);
	out << "}" << std::endl;
	return out; 
}

