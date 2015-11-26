#pragma once

#include <list>
#include <memory>
#include <ostream>
#include <string>
#include <boost/type_erasure/any.hpp>

namespace br {

	class Node {
	public:
		virtual ~Node() noexcept;

		virtual void invoke() const = 0;

		virtual void print(std::ostream & ostream, std::size_t indent = 0) const = 0;
	};

	class Expression : public Node {
	public:
		virtual ~Expression() noexcept;
	};

	class Variable : public Expression {
	public:
		Variable(
			std::string const & id
		) noexcept : m_id(id) {
		}

		virtual ~Variable() noexcept;

		virtual void invoke() const override;

		virtual void print(std::ostream & ostream, std::size_t indent = 0) const override;

	private:
		std::string m_id;
	};

	class Constant : public Expression {
	public:
		Constant(
				std::string const & id
		) noexcept : m_id(id) {
		}

		virtual ~Constant() noexcept;

		virtual void invoke() const override;

		virtual void print(std::ostream & ostream, std::size_t indent = 0) const override;

	private:
		std::string m_id;
	};

	class Numeric : public Expression {
	public:
		Numeric(
			double value
		) noexcept : m_value(value) {
		}

		virtual ~Numeric() noexcept;

		virtual void invoke() const override;

		virtual void print(std::ostream & ostream, std::size_t indent = 0) const override;

	private:
		double m_value;
	};

	class Boolean : public Expression {
	public:
		constexpr Boolean(
			bool value
		) noexcept : m_value(value) {
		}

		virtual ~Boolean() noexcept;

		virtual void invoke() const override;

		virtual void print(std::ostream & ostream, std::size_t indent = 0) const override;

	private:
		bool m_value;
	};

	class Vector : public Expression {
	public:
		Vector(
			std::shared_ptr<Expression> const & x,
			std::shared_ptr<Expression> const & y
		) noexcept : m_x(x), m_y(y) {
		}

		virtual ~Vector() noexcept;

		virtual void invoke() const override;

		virtual void print(std::ostream & ostream, std::size_t indent = 0) const override;

	private:
		std::shared_ptr<Expression> m_x, m_y;
	};

	class FunctionCall : public Expression {
	public:
		FunctionCall(
			std::shared_ptr<Expression> func,
			std::shared_ptr< std::list< std::shared_ptr<Expression> > > args
		) noexcept : m_func(func), m_args(args) {
		}

		virtual ~FunctionCall() noexcept;

		virtual void invoke() const override;

		virtual void print(std::ostream & ostream, std::size_t indent = 0) const override;

	private:
		std::shared_ptr<Expression> m_func;
		std::shared_ptr< std::list< std::shared_ptr<Expression> > > m_args;
	};

	class ArrayAccess : public Expression {
	public:
		ArrayAccess(
			std::shared_ptr<Expression> var,
			std::shared_ptr<Expression> index
		) noexcept : m_var(var), m_index(index) {
		}

		virtual ~ArrayAccess() noexcept;

		virtual void invoke() const override;

		virtual void print(std::ostream & ostream, std::size_t indent = 0) const override;

	private:
		std::shared_ptr<Expression> m_var;
		std::shared_ptr<Expression> m_index;
	};

	class UnaryOperation : public Expression {
	public:
		UnaryOperation(
			std::string const & op,
			std::shared_ptr<Expression> rhs
		) noexcept : m_op(op), m_rhs(rhs) {
		}

		virtual ~UnaryOperation() noexcept;

		virtual void invoke() const override;

		virtual void print(std::ostream & ostream, std::size_t indent = 0) const override;

	private:
		std::string m_op;
		std::shared_ptr<Expression> m_rhs;
	};

	class BinaryOperation : public Expression {
	public:
		BinaryOperation(
			std::string const & op,
			std::shared_ptr<Expression> lhs,
			std::shared_ptr<Expression> rhs
		) noexcept : m_op(op), m_lhs(lhs), m_rhs(rhs) {
		}

		virtual ~BinaryOperation() noexcept;

		virtual void invoke() const override;

		virtual void print(std::ostream & ostream, std::size_t indent = 0) const override;

	private:
		std::string m_op;
		std::shared_ptr<Expression> m_lhs;
		std::shared_ptr<Expression> m_rhs;
	};

	class Statement : public Node {
	public:
		virtual ~Statement() noexcept;
	};

	class EmptyStatement : public Statement {
	public:
		virtual ~EmptyStatement() noexcept;

		virtual void invoke() const override;

		virtual void print(std::ostream & ostream, std::size_t indent = 0) const override;
	};

	class CompoundStatement : public Statement {
	public:
		CompoundStatement(
			std::shared_ptr< std::list< std::shared_ptr<Statement> > > stmts
		) : m_stmts(stmts) {
		}

		virtual ~CompoundStatement() noexcept;

		virtual void invoke() const override;

		virtual void print(std::ostream & ostream, std::size_t indent = 0) const override;

	private:
		std::shared_ptr< std::list< std::shared_ptr<Statement> > > m_stmts;
	};

	class Program : public Statement {
	public:
		Program(
			std::shared_ptr< std::list< std::shared_ptr<Statement> > > stmts
		) : m_stmts(stmts) {
		}

		virtual ~Program() noexcept;

		virtual void invoke() const override;

		virtual void print(std::ostream & ostream, std::size_t indent = 0) const override;

	private:
		std::shared_ptr< std::list< std::shared_ptr<Statement> > > m_stmts;
	};

	class ConditionalStatement : public Statement {
	public:
		ConditionalStatement(
			std::shared_ptr<Expression> const & cond,
			std::shared_ptr<Statement> when_true,
			std::shared_ptr<Statement> when_false
		) : m_cond(cond), m_when_true(when_true), m_when_false(when_false) {
		}

		virtual ~ConditionalStatement() noexcept;

		virtual void invoke() const override;

		virtual void print(std::ostream & ostream, std::size_t indent = 0) const override;

	private:
		std::shared_ptr<Expression> m_cond;
		std::shared_ptr<Statement> m_when_true, m_when_false;
	};

	class WhileStatement : public Statement {
	public:
		WhileStatement(
			std::shared_ptr<Expression> cond,
			std::shared_ptr<Statement> body
		) : m_cond(cond), m_body(body) {
		}

		virtual ~WhileStatement() noexcept;

		virtual void invoke() const override;

		virtual void print(std::ostream & ostream, std::size_t indent = 0) const override;

	private:
		std::shared_ptr<Expression> m_cond;
		std::shared_ptr<Statement> m_body;
	};

	class UntilStatement : public Statement {
	public:
		UntilStatement(
			std::shared_ptr<Expression> cond,
			std::shared_ptr<Statement> body
		) : m_cond(cond), m_body(body) {
		}

		virtual ~UntilStatement() noexcept;

		virtual void invoke() const override;

		virtual void print(std::ostream & ostream, std::size_t indent = 0) const override;

	private:
		std::shared_ptr<Expression> m_cond;
		std::shared_ptr<Statement> m_body;
	};

	class ForStatement : public Statement {
	public:
		ForStatement(
			std::string const & id,
			std::shared_ptr<Expression> from,
			std::shared_ptr<Expression> to,
			std::shared_ptr<Expression> step,
			std::shared_ptr<Statement> body
		) : m_id(id), m_from(from), m_to(to), m_step(step), m_body(body) {
		}

		virtual ~ForStatement() noexcept;

		virtual void invoke() const override;

		virtual void print(std::ostream & ostream, std::size_t indent = 0) const override;

	private:
		std::string m_id;
		std::shared_ptr<Expression> m_from, m_to, m_step;
		std::shared_ptr<Statement> m_body;
	};

	class AssignStatement : public Statement {
	public:
		AssignStatement(
			std::string const & id,
			std::shared_ptr<Expression> expr
		) : m_id(id), m_expr(expr) {
		}

		virtual ~AssignStatement() noexcept;

		virtual void invoke() const override;

		virtual void print(std::ostream & ostream, std::size_t indent = 0) const override;

	private:
		std::string m_id;
		std::shared_ptr<Expression> m_expr;
	};

	class ExpressionStatement : public Statement {
	public:
		ExpressionStatement(std::shared_ptr<Expression> const & expr) : m_expr(expr) {
		}

		virtual ~ExpressionStatement() noexcept;

		virtual void invoke() const override;

		virtual void print(std::ostream & ostream, std::size_t indent = 0) const override;

	private:
		std::shared_ptr<Expression> m_expr;
	};


} // namespace br