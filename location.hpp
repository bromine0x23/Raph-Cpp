#pragma once

#include <string>
#include <ostream>
#include <memory>

namespace br {

	/// Abstract a position.
	class Position {
	public:
		/// Construct a position.
		explicit Position(std::shared_ptr<std::string> const & f = nullptr, unsigned int l = 1u, unsigned int c = 1u)
				: filename(f), line (l), column (c)
		{
		}

		/// Initialization.
		void initialize (std::shared_ptr<std::string> const & f = nullptr, unsigned int l = 1u, unsigned int c = 1u) {
			filename = f;
			line = l;
			column = c;
		}

		/** \name Line and Column related manipulators
		 ** \{ */
		/// (line related) Advance to the COUNT next lines.
		void lines(int count = 1) {
			if (count != 0) {
				column = 1u;
				line = add_(line, count, 1);
			}
		}

		/// (column related) Advance to the COUNT next columns.
		void columns(int count = 1) {
			column = add_(column, count, 1);
		}
		/** \} */

		/// Add \a width columns, in place.
		auto operator+=(int width) -> Position & {
			columns(width);
			return *this;
		}

		/// Add \a width columns.
		auto operator+(int width) -> Position {
			Position result = *this;
			result += width;
			return result;
		}

		/// Subtract \a width columns, in place.
		auto operator-=(int width) -> Position & {
			operator+=(-width);
			return *this;
		}

		/// Add \a width columns.
		auto operator-(int width) -> Position {
			Position result = *this;
			result -= width;
			return result;
		}

		auto operator==(Position const & other) const -> bool {
			return line == other.line && column == other.column && (
				filename == other.filename || (filename != nullptr && other.filename != nullptr && *filename == *other.filename)
			);
		}

		auto operator!=(Position const & other) const -> bool {
			return !operator==(other);
		}

		/// File name to which this position refers.
		std::shared_ptr<std::string> filename;
		/// Current line number.
		unsigned int line;
		/// Current column number.
		unsigned int column;

	private:
		/// Compute max(min, lhs+rhs) (provided min <= lhs).
		static auto add_(unsigned int lhs, int rhs, unsigned int min) -> unsigned int {
			return (0 < rhs || -static_cast<unsigned int>(rhs) < lhs ? rhs + lhs : min);
		}
	};// class Position

	template< typename TChar >
	inline auto operator<<(std::basic_ostream<TChar> & ostr, Position const & position) -> std::basic_ostream<TChar> & {
		if (position.filename != nullptr) {
			ostr << *position.filename << ':';
		}
		return ostr << position.line << '.' << position.column;
	}

	/// Abstract a location.
	class Location {
	public:
		/// Construct a location from \a b to \a e.
		Location(const Position & b, const Position & e) : begin(b), end(e) {
		}

		/// Construct a 0-width location in \a p.
		explicit Location(const Position & p = Position()) : begin(p), end(p) {
		}

		/// Construct a 0-width location in \a f, \a l, \a c.
		explicit Location(std::shared_ptr<std::string> const & f, unsigned int l = 1u, unsigned int c = 1u) : begin(f, l, c), end(f, l, c) {
		}

		/// Initialization.
		void initialize(std::shared_ptr<std::string> f = nullptr, unsigned int l = 1u, unsigned int c = 1u) {
			begin.initialize(f, l, c);
			end = begin;
		}

		/** \name Line and Column related manipulators
		 ** \{ */
	public:
		/// Reset initial location to final location.
		void step() {
			begin = end;
		}

		/// Extend the current location to the COUNT next columns.
		void columns(int count = 1) {
			end += count;
		}

		/// Extend the current location to the COUNT next lines.
		void lines(int count = 1) {
			end.lines(count);
		}
		/** \} */

	public:
		/// Join two locations, in place.
		auto operator+=(Location const & other) -> Location & {
			end = other.end;
			return *this;
		}

		auto operator+(Location const & other) const -> Location {
			Location result(*this);
			result += other;
			return result;
		}

		/// Add \a width columns to the end position, in place.
		auto operator+=(int width) -> Location & {
			columns(width);
			return *this;
		}

		auto operator+(int width) const -> Location {
			Location result(*this);
			result += width;
			return result;
		}

		/// Subtract \a width columns to the end position, in place.
		auto operator-=(int width) -> Location & {
			columns(-width);
			return *this;
		}

		auto operator-(int width) const -> Location {
			Location result(*this);
			result -= width;
			return result;
		}

		auto operator==(Location const & other) const -> bool {
			return begin == other.begin && end == other.end;
		}

		auto operator!=(Location const & other) const -> bool {
			return !operator==(other);
		}

	public:
		/// Beginning of the located region.
		Position begin;
		/// End of the located region.
		Position end;
	};

	/** \brief Intercept output stream redirection.
	 ** \param ostr the destination output stream
	 ** \param loc a reference to the location to redirect
	 **
	 ** Avoid duplicate information.
	 */
	template< typename TChar >
	inline auto operator<<(std::basic_ostream< TChar > & ostr, const Location & location) -> std::basic_ostream< TChar > & {
		unsigned int end_column = 0 < location.end.column ? location.end.column - 1 : 0;
		ostr << location.begin;
		if (location.end.filename != nullptr && (location.begin.filename == nullptr || *location.begin.filename != *location.end.filename)) {
			ostr << '-' << location.end.filename << ':' << location.end.line << '.' << end_column;
		} else if (location.begin.line < location.end.line) {
			ostr << '-' << location.end.line << '.' << end_column;
		} else if (location.begin.column < end_column) {
			ostr << '-' << end_column;
		}
		return ostr;
	}

} // br
