#include "utilities/Object.h"

using namespace interpreter;

void Object::AddTabs(std::string& s) {
	for (int i = 0; i < Object::nestedCounterPrint; i++) {
		s += "\t";
	}
}

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

bool Object::HasProperty(const Value& key) const {
	if (symbols.find(key) != symbols.end())
		return true;
	else
		return false;
}

std::string Object::toString() const {
	std::string s = "{\n";
	nestedCounterPrint++;
	int commaCounter = 0;
	for (auto& [key, value] : symbols) {
		AddTabs(s);
		s += key.toString() + " : " + value.toString();
		if (commaCounter++ < symbols.size() - 1)
			s += ", ";
		s += "\n";
	}
	nestedCounterPrint--;
	AddTabs(s);
	s += "}\n";
	return s;
}

void Object::IncreaseReferenceCounter() { ++referenceCounter; }
void Object::DecreaseReferenceCounter() { --referenceCounter; }

//overloads
Value Object::operator==(Object* obj) {
	return (this == obj);
}

Value  Object::operator!=(Object* obj) {
	return (this != obj);
}

std::ostream& interpreter::operator<< (std::ostream& out, const Object& obj) {
	return out << obj.toString();
	}