#pragma once

#include <string>
#include <unordered_map>
#include <iostream>
#include "utilities/Value.h"


#define map_t std::unordered_map<std::string, Value*> 
#define ASTnode Object
#define Environment Object

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

		Value* GetValue(std::string key);

		bool ContainsKey(std::string key);

		void PrintMap();

};



