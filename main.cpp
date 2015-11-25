#include <iostream>
#include <memory>
#include <typeinfo>
#include "raph.hpp"
#include "gen/parser.hpp"

int main() {

	br::Raph raph;

	// raph.trace_scanning = true;
	// raph.trace_parsing = true;

	auto result = raph.parse("test.raph");

	std::cerr << std::endl;

	if (result == 0) {
		std::cout << "SUCC" << std::endl;
		if (raph.program != nullptr) {
			raph.program->print(std::cout);
		} else {
			std::cerr << "Program is empty!" << std::endl;
		}
	}

/*
	raph.scan_begin();

	br::Parser::symbol_type symbol;

	for (;;) {
		auto symbol = yylex(raph);
		printf("%d\n", symbol.token());
		if (symbol.token() == br::Parser::token_type::TK_EOF) {
			break;
		}
	}

	raph.scan_end();
*/
	return 0;
}