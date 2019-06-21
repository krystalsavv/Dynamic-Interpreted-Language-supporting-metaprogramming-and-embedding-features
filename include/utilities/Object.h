#pragma once

#include <string>
#include <unordered_map>
#include <iostream>
#include "utilities/Value.h"


//#define map_t std::unordered_map<std::string, Value> 
#define map_t std::unordered_map<Value, Value> 

#define ASTnode		Object

class Object {
	map_t symbols;

	public:
		Object() = default;
		Object(const Value& key, const Value& value);
		
		Value& GetValue(const Value& key);

		bool ContainsKey(const Value&) const;

		//overloads
		Value operator==(Object* obj);
		Value operator!=(Object* obj);

		friend std::ostream& operator << (std::ostream& out, const Object& o);

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



