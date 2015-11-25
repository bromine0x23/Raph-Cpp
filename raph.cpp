#include <iostream>
#include "raph.hpp"
#include "gen/parser.hpp"

namespace br {

	Raph::Raph() {
		m_variables["origin"] = 0.0;
		m_variables["rotate"] = 0.0;
		m_variables["scale"] = 1.0;
	}

	Raph::~Raph() {
	}

	int Raph::parse(std::string const & filename) {
		this->filename = filename;
		scan_begin();
		Parser parser(*this);
		parser.set_debug_level(trace_parsing);
		int result = parser.parse();
		scan_end();
		return result;
	}

	void Raph::error(Location const & location, std::string const & message) const {
		std::cerr << "[Raph] " << location << ": " << message << std::endl;
	}

	void Raph::error(std::string const & message) const {
		std::cerr << "[Raph] " << message << std::endl;
	}

} // namespace br