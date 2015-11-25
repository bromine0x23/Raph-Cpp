#pragma once

#include <string>
#include <map>
#include <memory>
#include <list>
#include "ast.hpp"
#include "location.hpp"
#include "gen/parser.hpp"

namespace br {

	class Raph {
	public:
		Raph();

		virtual ~Raph();

		void scan_begin();

		void scan_end();

		int parse(std::string const & filename);

		void error(Location const & location, std::string const & message) const;

		void error(std::string const & message) const;

		std::string filename;

		bool trace_scanning = false;

		bool trace_parsing = false;

		std::shared_ptr<Program> program;

	private:
		std::map<std::string, double> m_variables;

		// std::map<std::string, Function> m_functions;
	};
} // namespace br

# define YY_DECL br::Parser::symbol_type yylex(br::Raph & raph)

YY_DECL;