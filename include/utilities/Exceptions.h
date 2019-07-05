#pragma once
#include <string>

namespace interpreter {

	class BreakException : public std::exception
	{
		std::string message;
	public:

		BreakException::BreakException(std::string str) {
			message = "Break Exception: " + str + "\n";
		}

		BreakException::BreakException() {
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

		ContinueException::ContinueException(std::string str) {
			message = "Continue Exception: " + str + "\n";
		}

		ContinueException::ContinueException() {
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

		ReturnException::ReturnException(std::string str) {
			message = "Return Exception: " + str + "\n";
		}

		ReturnException::ReturnException() {
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

		ReturnValueException::ReturnValueException(std::string str) {
			message = "Return value Exception: " + str + "\n";
		}

		ReturnValueException:: ReturnValueException() {
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

		RuntimeErrorException::RuntimeErrorException(std::string str) {
			message = "Runtime Error: " + str + "\n";
		}

		RuntimeErrorException::RuntimeErrorException() {
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

		SyntaxErrorException::SyntaxErrorException(std::string str) {
			message = "Syntax Error: " + str + "\n";
		}

		SyntaxErrorException::SyntaxErrorException() {
			message = "Syntax Error\n";
		}

		const char* what() const noexcept override {
			return  message.c_str();
		}
	};

}

