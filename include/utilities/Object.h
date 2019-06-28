#pragma once

#include <string>
#include <unordered_map>
#include <iostream>
#include "utilities/Value.h"
#define map_t std::unordered_map<Value, Value> 

namespace interpreter {

	class Object {
		map_t symbols;
		unsigned int referenceCounter = 0;
		inline static unsigned int nestedCounterPrint = 0;
		static void AddTabs(std::string& s);
	public:
		Object() = default;
		Object(const Value& key, const Value& value);

		const Value& GetValue(const Value& key) const;
		void Set(const Value& key, const Value& value);

		bool ContainsKey(const Value& key) const;
		std::string toString() const;

		//overloads
		Value operator==(Object* obj);
		Value operator!=(Object* obj);
		friend std::ostream& operator<< (std::ostream& out, const Object& obj);
	};	
}

