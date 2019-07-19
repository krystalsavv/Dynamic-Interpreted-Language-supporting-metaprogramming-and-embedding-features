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


Object::Object(const Object& obj) {
	symbols = obj.symbols;
}



Object::~Object() {
	//std::cout << "destroyedBlocks: " << ++destroyedBlocks << std::endl;
}


map_t Object::GetMap() {
	return symbols;
}

Value* Object::GetValue(const Value& key) {
	auto value = symbols.find(key);
	if (value != symbols.end())
		return &(value->second);
	else
		return nullptr;	
}

void Object::Set(const Value& key, const Value& value) {

	//if (this && HasProperty(key) && GetValue(key)->isObject() && GetValue(key)->GetObjectValue())
	//	DecreaseReferenceCounter();

	if (value.isObject() && value.GetObjectValue())
		value.GetObjectValue()->IncreaseReferenceCounter();

	symbols[key] = value;

}

bool Object::HasProperty(const Value& key) const {
	if (symbols.find(key) != symbols.end())
		return true;
	else
		return false;
}

size_t interpreter::Object::size() const
{
	return symbols.size();
}

std::string Object::toString()  {
	isPrinted = true;
	nestedCounterPrint++;
	int commaCounter = 0;
	std::string s = "{\n";
	for (auto& [key, value] : symbols) {
		AddTabs(s);
		if (value.isObject() && value.GetObjectValue() != nullptr && value.GetObjectValue()->isPrinted == true) 
			s += key.toString() + " : " + "same object as printing";
		else 
			s += key.toString() + " : " + value.toString();
		if (commaCounter++ < symbols.size() - 1)
			s += ", ";
		s += "\n";
		//std::cout << s;
	}
	nestedCounterPrint--;
	AddTabs(s);
	s += "}\n";
	s += std::to_string(GetReferenceCounter());
	isPrinted = false;
	return s;
}

void Object::IncreaseReferenceCounter() { 
	++referenceCounter; 
	/*std::cout << std::endl << "#####################INCREASED##############################\n";
	std::cout << *this << std::endl << referenceCounter << std::endl;
	std::cout << "###################################################\n";*/
}

void Object::DecreaseReferenceCounter() { 
	--referenceCounter;
	assert(referenceCounter >= 0);
	/*std::cout << std::endl << "#####################DECREASED##############################\n";
	std::cout << *this << std::endl << referenceCounter << std::endl;
	std::cout << "###################################################\n";*/
	if (referenceCounter == 0) {
		ClearObject(this);
		delete this;
	}
}

long long int Object::GetReferenceCounter()
{
	/*std::cout << std::endl << "###################################################\n";
	std::cout  << *this << std::endl << referenceCounter <<std::endl;
	std::cout << "###################################################\n";*/
	return referenceCounter;
}

//overloads
Value Object::operator==(Object* obj) {
	return (this == obj);
}

Value  Object::operator!=(Object* obj) {
	return (this != obj);
}

std::ostream& interpreter::operator<< (std::ostream& out, Object& obj) {
	return out << obj.toString();
	}

std::string Object::GenerateAnonymousName()
{
	std::string name = "$f" + std::to_string(anonymousFuncCounter);
	anonymousFuncCounter++;
	return name;
}

void interpreter::ClearObject(Object* obj) {
	//if (!obj) return;
	assert(obj);
	//if (obj->GetReferenceCounter() > 0)	return;
	for (auto pair : obj->GetMap()) {
		if (pair.first.isObject() && pair.first.GetObjectValue())
			pair.first.GetObjectValue()->DecreaseReferenceCounter();
		if (pair.second.isObject() && pair.second.GetObjectValue())
			pair.second.GetObjectValue()->DecreaseReferenceCounter();
	}
}
