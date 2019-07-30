#pragma once
#include <string>

namespace interpreter {

	extern size_t lineno;

	class BreakException : public std::exception
	{
		std::string message;
	public:

		BreakException(std::string str) {
			message = "Error: Break outside of loop at line " + std::to_string(lineno);
		}

		BreakException() {
			message = "Error: Break outside of loop at line " + std::to_string(lineno);
		}

		const char* what() const noexcept override {
			return message.c_str();
		}
	};

	class ContinueException : public std::exception
	{
		std::string message;
	public:

		ContinueException(std::string str) {
			message = "Error: Continue outside of loop at line " + std::to_string(lineno);
		}

		ContinueException() {
			message = "Error: Continue outside of loop at line " + std::to_string(lineno);
		}

		const char* what() const noexcept override {
			return  message.c_str();
		}
	};

	class ReturnException : public std::exception
	{
		std::string message;
	public:

		ReturnException(std::string str) {
			message = "Error: Return outside of function at line " + std::to_string(lineno);
		}

		ReturnException() {
			message = "Error: Return outside of function at line " + std::to_string(lineno);
		}

		const char* what() const noexcept override {
			return message.c_str();
		}
	};

	class ReturnValueException : public std::exception
	{
		std::string message;
	public:

		ReturnValueException(std::string str) {
			message = "Error: Return outside of function at line " + std::to_string(lineno);
		}

		ReturnValueException() {
			message = "Error: Return outside of function at line " + std::to_string(lineno);
		}

		const char* what() const noexcept override {
			return message.c_str();
		}
	};

	class RuntimeErrorException : public std::exception
	{
		std::string message;
	public:

		RuntimeErrorException(std::string str) {
			message = "Runtime Error: " + str + " at line " + std::to_string(lineno);
		}

		RuntimeErrorException() {
			message = "Runtime Error at line " + std::to_string(lineno);
		}

		const char* what() const noexcept override {
			return message.c_str();
		}
	};

	class SyntaxErrorException : public std::exception
	{
		std::string message;
	public:

		SyntaxErrorException(std::string str) {
			message = "Syntax Error: " + str + " at line " + std::to_string(lineno);
		}

		SyntaxErrorException() {
			message = "Syntax Error at line" + std::to_string(lineno);
		}

		const char* what() const noexcept override {
			return  message.c_str();
		}
	};

}

