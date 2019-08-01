#pragma once
#include <string>

namespace interpreter {


	extern size_t lineNumber;

	#define WARNING(msg)  std::cout << "Warning: " << msg << " at line " << lineNumber << std::endl; 
 



	class BreakException : public std::exception
	{
		std::string message;
	public:

		BreakException(std::string str) {
			message = "Error: Break outside of loop at line " + std::to_string(lineNumber);
		}

		BreakException() {
			message = "Error: Break outside of loop at line " + std::to_string(lineNumber);
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
			message = "Error: Continue outside of loop at line " + std::to_string(lineNumber);
		}

		ContinueException() {
			message = "Error: Continue outside of loop at line " + std::to_string(lineNumber);
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
			message = "Error: Return outside of function at line " + std::to_string(lineNumber);
		}

		ReturnException() {
			message = "Error: Return outside of function at line " + std::to_string(lineNumber);
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
			message = "Error: Return outside of function at line " + std::to_string(lineNumber);
		}

		ReturnValueException() {
			message = "Error: Return outside of function at line " + std::to_string(lineNumber);
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
			message = "Runtime Error: " + str + " at line " + std::to_string(lineNumber);
		}

		RuntimeErrorException() {
			message = "Runtime Error at line " + std::to_string(lineNumber);
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
			message = "Syntax Error: " + str + " at line " + std::to_string(lineNumber);
		}

		SyntaxErrorException() {
			message = "Syntax Error at line" + std::to_string(lineNumber);
		}

		const char* what() const noexcept override {
			return  message.c_str();
		}
	};

}

