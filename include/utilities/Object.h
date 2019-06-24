#pragma once

#include <string>
#include <unordered_map>
#include <iostream>
#include "utilities/Value.h"


#define map_t std::unordered_map<Value, Value> 

#define ASTnode		Object

class Object {
	map_t symbols;
	unsigned referenceCounter = 0;
	static unsigned int nestedCounterPrint;
	static void PrintTabs(std::ostream& out);
	public:
		Object() = default;
		Object(const Value& key, const Value& value);
		
		const Value& GetValue(const Value& key) const;
		void Set(const Value& key, const Value& value);

		bool ContainsKey(const Value& key) const;

		
		//overloads
		Value operator==(Object* obj);
		Value operator!=(Object* obj);
		friend std::ostream& operator << (std::ostream& out, const Object& obj);
};



