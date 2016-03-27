#ifndef CONFIG_NULL_EXCEPTION_H
#define CONFIG_NULL_EXCEPTION_H

#include <iostream>
#include <exception>
#include <stdexcept>

class ConfigNullException : public std::runtime_error {
public:
	ConfigNullException()
		: std::runtime_error("Error with configuration of acoustic and language model.")
	{ }

	const char* what() const throw() {
		return std::runtime_error::what();
	}
};


#endif