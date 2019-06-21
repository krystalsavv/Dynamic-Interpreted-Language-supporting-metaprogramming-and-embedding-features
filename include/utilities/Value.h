#pragma once

#include <variant>
#include <string>
#include <iostream>
#include <cassert>

class Object;

#define variant_t std::variant<bool, double, std::string, Object*> 

class Value
{

	variant_t variant;

public:
	Value() = default;
	Value( bool value);
	Value( double value);
	Value(std::string value);
	Value( const char * value);
	Value( Object* value);

	const bool& GetBoolValue() const;
	const double& GetNumberValue() const;
	const std::string& GetStringValue() const;
	Object* GetObjectValue() const;
	
	bool toBool();

	bool isBool() const;
	bool isNumber()const;
	bool isString() const;
	bool isObject() const;

	void PrintValue() const;

	//template set
	template <class T>
	void  Set(T value) {
		variant =value;
	}

	template <>
	void  Set<const char *>(const char * value) {
		variant = std::string(value);
	}

	//template <>
	//void  Set<Value>(Value value) {
	//	variant = std::string(value);
	//}



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

	//Value operator>(const Value& right) const;
	//Value operator>=(const Value& right) const;
	//Value operator<(const Value& right) const;
	//Value operator<=(const Value& right) const;


	Value operator==(Value& right);
	Value operator!=(Value& right);
	//for consts (unordered map comparison)
	//Value operator==(const Value& right) const;
	//Value operator!=(const Value& right) const;

	//logical
	Value operator&&(Value& right);
	Value operator||(Value& right);
	Value operator!();

};

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