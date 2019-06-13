#include "utilities/Object.h"


Object::Object(std::string key, bool value) {
	variant_t v = value;
	symbols[key] = v;
}

Object::Object(std::string key, double value) {
	variant_t v = value;
	symbols[key] = v;
}

Object::Object(std::string key, std::string value) {
	variant_t v = value;
	symbols[key] = v;
}

Object::Object(std::string key, Object* value) {
	variant_t v = value;
	symbols[key] = v;
}

void Object::Set(std::string key, bool value) {
	variant_t v = value;
	symbols[key] = v;
}

void Object::Set(std::string key, double value) {
	variant_t v = value;
	symbols[key] = v;
}

void Object::Set(std::string key, std::string value) {
	variant_t v = value;
	symbols[key] = v;
}

void Object::Set(std::string key, Object* value) {
	variant_t v = value;
	symbols[key] = v;
}


bool& Object::GetBoolValue(std::string key) {
	return std::get<bool>(symbols[key]);
}

double& Object::GetNumberValue(std::string key) {
	return std::get<double>(symbols[key]);
}

std::string& Object::GetStringValue(std::string key) {
	return std::get<std::string>(symbols[key]);
}

Object* Object::GetObjectValue(std::string key) {
	return std::get<Object*>(symbols[key]);
}

bool Object::ContainsKey(std::string key) {
	if (symbols.find(key) != symbols.end())
		return true;
	else
		return false;
}

bool Object::isBool(std::string key) {
	return std::holds_alternative<Object*>(symbols[key]);
}

bool Object::isNumber(std::string key) {
	return std::holds_alternative<double>(symbols[key]);
}

bool Object::isString(std::string key) {
	return std::holds_alternative<std::string>(symbols[key]);
}

bool Object::isObject(std::string key) {
	return std::holds_alternative<Object*>(symbols[key]);
}

//auto& Object::GetValue(std::string& key) {
//	if (symbols.find(key) != symbols.end()) {
//
//		if (std::holds_alternative<int>(symbols[key]))
//			return std::get<int>(symbols[key]);
//		else if (std::holds_alternative<double>(symbols[key]))
//			return std::get<double>(symbols[key]);
//		else if (std::holds_alternative<std::string>(symbols[key]))
//			return std::get<std::string>(symbols[key]);
//		else if (std::holds_alternative<Object*>(symbols[key]))
//			return std::get<Object*>(symbols[key]);
//	}
//	return key;
//}

void Object::PrintMap() {
	for (auto& [key, control] : symbols) {
		std::visit([key](auto&& c) {
			std::cout << "key: " << key << " value: " << c << std::endl;
			}, control);
	}
}

