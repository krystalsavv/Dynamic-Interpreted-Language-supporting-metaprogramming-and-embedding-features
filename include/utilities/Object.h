#pragma once

#include <string>
#include <unordered_map>
#include <iostream>
#include "utilities/Value.h"


#define map_t std::unordered_map<std::string, Value*> 
#define ASTnode Object
#define Environment Object

class Object {
	map_t symbols;

	public:
		Object() = default;
		Object(std::string key, bool value);
		Object(std::string key, double value);
		Object(std::string key, std::string value);
		Object(std::string key, Object* value);
		
		Value* GetValue(std::string key);

		bool ContainsKey(std::string key);

		void PrintMap();

		template <class T>
		void Set(std::string key, T value) {
			if (symbols[key])
				symbols[key]->Set(value);
			else
				symbols[key] = new Value(value);
		}
};



