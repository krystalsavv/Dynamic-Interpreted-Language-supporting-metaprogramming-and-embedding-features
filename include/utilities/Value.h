#pragma once

#include <variant>
#include <string>
#include <iostream>
#include <cassert>
#include <iostream> 

#define variant_t std::variant<bool, double, std::string, Object*> 

namespace interpreter {

	class Object;

	class Value
	{
		variant_t variant;

	public:
		Value() = default;
		Value(bool value);
		Value(double value);
		Value(std::string value);
		Value(const char* value);
		Value(Object* value);
		Value(const Value& value);

		const bool& GetBoolValue() const;
		const double& GetNumberValue() const;
		const std::string& GetStringValue() const;
		Object* GetObjectValue() const;

		bool toBool() const;
		std::string toString() const;

		bool isBool() const;
		bool isNumber()const;
		bool isString() const;
		bool isObject() const;

		void Set(const Value& value);

		//overloads
		Value operator+(Value& right);
		Value operator-(Value& right);
		Value operator*(Value& right);
		Value operator/(Value& right);
		Value operator%(Value& right);
		//prefix
		Value& operator++();
		//postfix
		Value operator++(int);
		//prefix
		Value& operator--();
		//postfix
		Value operator--(int);
		Value operator-();

		//equals
		Value operator>(Value& right);
		Value operator>=(Value& right);
		Value operator<(Value& right);
		Value operator<=(Value& right);
		Value operator==(Value& right);
		Value operator!=(Value& right);

		//for consts (unordered map comparison)
		bool operator==(const Value& right) const;
		bool operator!=(const Value& right) const;

		//logical
		Value operator&&(Value& right);
		Value operator||(Value& right);
		Value operator!();

		friend std::ostream& operator<< (std::ostream& out, const Value& value);
	};
}

using namespace interpreter;

namespace std
{
	template<> struct hash<Value>
	{
		typedef Value argument_type;
		typedef std::size_t result_type;
		result_type operator()(Value const& v) const noexcept
		{
			if (v.isBool()) {
				return std::hash<bool>{}(v.GetBoolValue());
			}
			else if (v.isNumber()) {
				return std::hash<double>{}(v.GetNumberValue());
			}
			else if (v.isString()) {
				return std::hash<std::string>{}(v.GetStringValue());
			}
			else {
				return std::hash<Object*>{}(v.GetObjectValue());
			}
		}
	};
}