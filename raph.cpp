#include <cerrno>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include "raph.hpp"
#include "gen/parser.hpp"

namespace br {

	RaphParser::RaphParser(std::string const & filename, bool trace_scanning, bool trace_parsing) : m_filename(filename), m_trace_scanning(trace_scanning), m_trace_parsing(trace_parsing) {
	}

	RaphParser::~RaphParser() {
	}

	std::shared_ptr<Program> RaphParser::parse() const {
		YYScan scanner;
		yylex_init(&scanner);
		yyset_debug(m_trace_scanning, scanner);
		auto closer = [](FILE * file) {
			if (file != stdin) {
				std::fclose(file);
			}
		};
		std::unique_ptr< FILE, decltype(closer) > file(stdin, closer);
		if (!filename().empty() && filename() != "-") {
			file.reset(std::fopen(filename().c_str(), "r"));
			if (file == nullptr) {
				this->error("cannot open " + filename() + ": " + strerror(errno));
				std::exit(EXIT_FAILURE);
			}
		}
		yyset_in(file.get(), scanner);

		std::shared_ptr<br::Program> program;

		Parser parser(scanner, *this, program);
		parser.set_debug_level(m_trace_parsing);
		parser.parse();

		yylex_destroy(scanner);
		return program;
	}

	void RaphParser::error(Location const & location, std::string const & message) const {
		std::cerr << "[Raph] " << location << ": " << message << std::endl;
	}

	void RaphParser::error(std::string const & message) const {
		std::cerr << "[Raph] " << message << std::endl;
	}

} // namespace br