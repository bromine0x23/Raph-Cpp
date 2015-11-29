#include <iostream>
#include <memory>
#include <typeinfo>
#include "raph.hpp"

int main() {

	br::RaphParser parser("test.raph");

	auto program = parser.parse();

	std::cerr << std::endl;

	if (program != nullptr) {
		program->print(std::cout);
	} else {
		std::cerr << "Program is empty!" << std::endl;
	}

	return 0;
}