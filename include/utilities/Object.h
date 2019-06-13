#pragma once
#include <string>
#include <unordered_map>
#include <iostream>
#include <variant>

#define variant_t std::variant<bool, double, std::string, Object*> 
#define map_t std::unordered_map<std::string, variant_t> 
#define ASTnode Object

class Object {
	std::string type;
	map_t symbols;

	public:
		Object() = default;
		Object(std::string key, bool value);
		Object(std::string key, double value);
		Object(std::string key, std::string value);
		Object(std::string key, Object* value);
		//auto& GetValue(std::string& key);

		void Set(std::string key, bool value);
		void Set(std::string key, double value);
		void Set(std::string key, std::string value);
		void Set(std::string key, Object* value);

		bool& GetBoolValue(std::string key);
		double& GetNumberValue(std::string key);
		std::string& GetStringValue(std::string key);
		Object* GetObjectValue(std::string key);

		bool isBool(std::string key);
		bool isNumber(std::string key);
		bool isString(std::string key);
		bool isObject(std::string key);

		bool ContainsKey(std::string key);

		void PrintMap();

};



