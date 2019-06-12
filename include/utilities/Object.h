#pragma once
#include <string>
#include <unordered_map>
#include <iostream>
#include <variant>

#define variant_t std::variant<int, double, std::string, Object*> 
#define map_t std::unordered_map<std::string, variant_t> 

class Object {
	std::string type;
	map_t symbols;

	public:
		Object() = default;
		Object(std::string key, int value);
		Object(std::string key, double value);
		Object(std::string key, std::string value);
		Object(std::string key, Object* value);
		//auto& GetValue(std::string& key);

		void Set(std::string key, int value);
		void Set(std::string key, double value);
		void Set(std::string key, std::string value);
		void Set(std::string key, Object* value);
		void PrintMap();

};


