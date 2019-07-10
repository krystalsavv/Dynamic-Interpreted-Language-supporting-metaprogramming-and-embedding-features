#pragma once
#include <string>

namespace interpreter {

	class BreakException : public std::exception
	{
		std::string message;
	public:

		BreakException(std::string str) {
			message = "Break Exception: " + str + "\n";
		}

		BreakException() {
			message = "Break Exception\n";
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
			message = "Continue Exception: " + str + "\n";
		}

		ContinueException() {
			message = "Continue Exception\n";
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
			message = "Return Exception: " + str + "\n";
		}

		ReturnException() {
			message = "Return Exception\n";
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
			message = "Return value Exception: " + str + "\n";
		}

		ReturnValueException() {
			message = "Return value Exception\n";
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
			message = "Runtime Error: " + str + "\n";
		}

		RuntimeErrorException() {
			message = "Runtime Error\n";
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
			message = "Syntax Error: " + str + "\n";
		}

		SyntaxErrorException() {
			message = "Syntax Error\n";
		}

		const char* what() const noexcept override {
			return  message.c_str();
		}
	};

}

