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

Value* Object::GetValue(const Value& key) {
	auto value = symbols.find(key);
	if (value != symbols.end())
		return &(value->second);
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

std::string Object::GenerateAnonymousName()
{
	int counter = 0, tmp = anonymousFuncCounter;
	while (anonymousFuncCounter > 0) {
		counter++;
		anonymousFuncCounter = anonymousFuncCounter - anonymousFuncCounter % 10;
		anonymousFuncCounter = anonymousFuncCounter / 10;
	}
	std::string name = "$f";
	std::string c;
	while (counter > 0) {
		int x = tmp % 10;
		if (x == 0) c = "0";
		else if (x == 1) c = "1";
		else if (x == 2) c = "2";
		else if (x == 3) c = "3";
		else if (x == 4) c = "4";
		else if (x == 5) c = "5";
		else if (x == 6) c = "6";
		else if (x == 7) c = "7";
		else if (x == 8) c = "8";
		else if (x == 9) c = "9";
		else c = "";
		tmp = tmp - tmp % 10;
		tmp = tmp / 10;
		name += c;
		counter--;
	}
	return name;
}