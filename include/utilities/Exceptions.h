#pragma once

namespace interpreter {

	class BreakException : public std::exception
	{
	public:
		const char* what() const noexcept override {
			return "A break exception for loops";
		}
	};

	class ContinueException : public std::exception
	{
	public:
		const char* what() const noexcept override {
			return "A continue exception for loops";
		}
	};

	class ReturnException : public std::exception
	{
	public:
		const char* what() const noexcept override {
			return "A return exception without value ";
		}
	};

	class ReturnValueException : public std::exception
	{
	public:
		const char* what() const noexcept override {
			return "A return exception with value";
		}
	};

}

