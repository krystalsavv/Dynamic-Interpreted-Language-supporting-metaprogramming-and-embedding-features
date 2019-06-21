#pragma once

#include <string>
#include <unordered_map>
#include <iostream>
#include "utilities/Value.h"


#define map_t std::unordered_map<Value, Value> 

#define ASTnode		Object

class Object {
	map_t symbols;

	public:
		Object() = default;
		Object(std::string key, bool value);
		Object(std::string key, double value);
		Object(std::string key, std::string value);
		Object(std::string key, const char * value);
		Object(std::string key, Object* value);
		
		Value& GetValue(std::string key);

		bool ContainsKey(std::string key) const;

		void PrintMap() const;

		//overloads
		Value operator==(Object* obj);
		Value operator!=(Object* obj);

		template <class T>
		void Set(std::string key, T value) {
				symbols[key].Set(value);
		}

		template <>
		void Set<const char *>(std::string key, const char * value) {
			symbols[key].Set(std::string(value));
		}

		template <>
		void Set<Value>(std::string key, Value value) {
			symbols[key] = value;
		}
};



