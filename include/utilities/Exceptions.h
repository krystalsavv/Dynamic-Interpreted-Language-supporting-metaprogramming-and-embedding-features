#pragma once
#include <string>

namespace interpreter {

	class BreakException : public std::exception
	{
		std::string message;
	public:

		BreakException(std::string str) {
			message = "Error: Break outside of loop";
		}

		BreakException() {
			message = "Error: Break outside of loop ";
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
			message = "Error: Continue outside of loop ";
		}

		ContinueException() {
			message = "Error: Continue outside of loop ";
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
			message = "Error: Return outside of function ";
		}

		ReturnException() {
			message = "Error: Return outside of function ";
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
			message = "Error: Return outside of function ";
		}

		ReturnValueException() {
			message = "Error: Return outside of function ";
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
			message = "Runtime Error: " + str;
		}

		RuntimeErrorException() {
			message = "Runtime Error: ";
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
			message = "Syntax Error: " + str;
		}

		SyntaxErrorException() {
			message = "Syntax Error: ";
		}

		const char* what() const noexcept override {
			return  message.c_str();
		}
	};

}

