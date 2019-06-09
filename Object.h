#pragma once
#include <string>
#include <unordered_map>
#include <iostream>
#include <variant>

#define variant_t std::variant<int, std::string, double, Object*> 
#define map_t std::unordered_map<std::string, variant_t> 

class Object {

	protected:
		std::unordered_map<std::string, variant_t> symbols;

	public:

		Object(std::string key, int value) {
			variant_t v = value;
			symbols[key] = v;
		}

		void PrintMap() {
			for (auto& [key, control] : symbols) {
				std::visit([](auto&& c) {
					std::cout << c;
					}, control);
			}
		}

};


