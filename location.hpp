#pragma once

#include <cstddef>
#include <string>
#include <ostream>
#include <memory>

namespace br {

	/// Abstract a position.
	class Position {
	public:
		/// Construct a position.
		explicit Position(std::shared_ptr<std::string> const & filename = nullptr, unsigned int line = 1, unsigned int column = 1)
				: filename(filename), line(line), column(column)
		{
		}

		/** \name Line and Column related manipulators
		 ** \{ */
		/// (line related) Advance to the COUNT next lines.
		void lines(int count = 1) {
			if (count != 0) {
				column = 1;
				line = m_add(line, count, 1);
			}
		}

		/// (column related) Advance to the COUNT next columns.
		void columns(int count = 1) {
			column = m_add(column, count, 1);
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
			return line == other.line && column == other.column &&
					(filename == other.filename || (filename != nullptr && other.filename != nullptr && *filename == *other.filename)
			);
		}

		auto operator!=(Position const & other) const -> bool {
			return !operator==(other);
		}

	public:
		/// File name to which this position refers.
		std::shared_ptr<std::string> filename;
		/// Current line number.
		unsigned int line;
		/// Current column number.
		unsigned int column;

	private:
		/// Compute max(min, lhs+rhs) (provided min <= lhs).
		static auto m_add(unsigned int lhs, int rhs, unsigned int min) -> unsigned int {
			return (0 < rhs || -static_cast<unsigned int>(rhs) < lhs ? rhs + lhs : min);
		}
	};// class Position

	template< typename TChar >
	inline auto operator<<(std::basic_ostream<TChar> & stream, Position const & position) -> std::basic_ostream<TChar> & {
		if (position.filename != nullptr) {
			stream << *position.filename << ':';
		}
		return stream << position.line << '.' << position.column;
	}

	/// Abstract a location.
	class Location {
	public:
		/// Construct a location from \a b to \a e.
		Location(Position const & begin, Position const & end) : begin(begin), end(end) {
		}

		/// Construct a 0-width location in \a p.
		explicit Location(Position const & position = Position()) : begin(position), end(position) {
		}

		/// Construct a 0-width location in \a f, \a l, \a c.
		explicit Location(
			std::shared_ptr<std::string> const & filename,
			unsigned int line = 1,
			unsigned int column = 1
		) : begin(filename, line, column), end(filename, line, column) {
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
	inline auto operator<<(std::basic_ostream< TChar > & stream, Location const & location) -> std::basic_ostream< TChar > & {
		unsigned int end_column = 0 < location.end.column ? location.end.column - 1 : 0;
		stream << location.begin;
		if (location.end.filename != nullptr && (location.begin.filename == nullptr || *location.begin.filename != *location.end.filename)) {
			stream << '-' << location.end.filename << ':' << location.end.line << '.' << end_column;
		} else if (location.begin.line < location.end.line) {
			stream << '-' << location.end.line << '.' << end_column;
		} else if (location.begin.column < end_column) {
			stream << '-' << end_column;
		}
		return stream;
	}

} // br
