#pragma once

#include <string>
#include <memory>
#include "ast.hpp"
#include "location.hpp"

namespace br {

	class RaphParser {
	public:
		RaphParser(std::string const & filename, bool trace_scanning = false, bool trace_parsing = false);

		virtual ~RaphParser();

		auto filename() const -> std::string const & {
			return m_filename;
		}

		std::shared_ptr<Program> parse() const;

		void error(Location const & location, std::string const & message) const;

		void error(std::string const & message) const;

	private:
		std::string m_filename;

		bool m_trace_scanning;

		bool m_trace_parsing;
	}; // class RaphParser
} // namespace br