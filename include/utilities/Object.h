#pragma once

#include <string>
#include <unordered_map>
#include <iostream>
#include "utilities/Value.h"
#define map_t std::unordered_map<Value, Value> 

namespace interpreter {

	class Object {
		map_t symbols;
		long long int referenceCounter = 0;
		inline static unsigned int nestedCounterPrint = 0;
		inline static unsigned long long int anonymousFuncCounter = 0;
		inline static unsigned long long int destroyedBlocks = 0;
		bool isPrinted = false;
		static void AddTabs(std::string& s);
	public:
		Object() = default;
		Object(const Value& key, const Value& value);
		Object(const Object& obj); 
		~Object();

		map_t GetMap();
		Value* GetValue(const Value& key);
		
		void Set(const Value& key, const Value& value);

		bool HasProperty(const Value& key) const;
		size_t size() const;
		std::string toString();

		void IncreaseReferenceCounter();
		void DecreaseReferenceCounter();
		long long int GetReferenceCounter();

		Value operator==(Object* obj);
		Value operator!=(Object* obj);
		friend std::ostream& operator<< (std::ostream& out,  Object& obj);

		static std::string GenerateAnonymousName();
	};	

	void ClearObject(Object* obj);

	//void DestroyObject(Object* obj);
	//void VisitAndPrint(Object* obj);

}

