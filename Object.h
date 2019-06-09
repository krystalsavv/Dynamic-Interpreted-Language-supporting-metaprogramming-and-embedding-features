#pragma once
#include <string>
#include <unordered_map>
#include <variant>

#define variant_t std::variant<int, std::string, double, Object *>


class Object {
protected:
	std::unordered_map<std::string, variant_t> symbols;
};

