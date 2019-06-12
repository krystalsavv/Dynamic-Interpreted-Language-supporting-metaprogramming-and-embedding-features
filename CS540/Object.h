#pragma once
#include <string>
#include <unordered_map>
#include <iostream>
#include <variant>

#define variant_t std::variant<int, double, std::string, Object*> 
#define map_t std::unordered_map<std::string, variant_t> 

class Object {

	protected:
		std::string type;
		std::unordered_map<std::string, variant_t> symbols;

	public:
		Object(std::string key, int value);
		Object(std::string key, double value);
		Object(std::string key, std::string value);
		Object(std::string key, Object* value);
		//auto& GetValue(std::string& key);
		void PrintMap();

};


