#include "ast.hpp"

namespace br {

	Node::~Node() noexcept {
	}

	Expression::~Expression() noexcept {
	}

	Variable::~Variable() noexcept {
	}

	void Variable::invoke() const {
		// TODO: m_id;
	}

	void Variable::print(std::ostream & ostream, std::size_t indent) const {
		ostream << std::string(indent, '\t') << "<Variable>: " << m_id << std::endl;
	}

	Constant::~Constant() noexcept {
	}

	void Constant::invoke() const {
		// TODO: m_id;
	}

	void Constant::print(std::ostream & ostream, std::size_t indent) const {
		ostream << std::string(indent, '\t') << "<Constant>: " << m_id << std::endl;
	}

	Numeric::~Numeric() noexcept {
	}

	void Numeric::invoke() const {
		// TODO: m_value;
	}

	void Numeric::print(std::ostream & ostream, std::size_t indent) const {
		ostream << std::string(indent, '\t') << "<Numeric>: " << m_value << std::endl;
	}

	Boolean::~Boolean() noexcept {
	}

	void Boolean::invoke() const {
		// TODO: m_value;
	}

	void Boolean::print(std::ostream & ostream, std::size_t indent) const {
		ostream << std::string(indent, '\t') << "<Boolean>: " << std::boolalpha << m_value << std::endl;
	}

	Vector::~Vector() noexcept {
	}

	void Vector::invoke() const {
		// TODO: (m_x, m_y);
	}

	void Vector::print(std::ostream & ostream, std::size_t indent) const {
		ostream << std::string(indent, '\t') << "<Vector>" << std::endl;
		m_x->print(ostream, indent + 1);
		m_y->print(ostream, indent + 1);
	}

	FunctionCall::~FunctionCall() noexcept {
	}

	void FunctionCall::invoke() const {
		// TODO: m_id(m_args);
	}

	void FunctionCall::print(std::ostream & ostream, std::size_t indent) const {
		ostream << std::string(indent, '\t') << "<FunctionCall>: " << std::endl;
		m_func->print(ostream, indent + 1);
		for (auto const & arg : *m_args) {
			arg->print(ostream, indent + 1);
		}
	}

	ArrayAccess::~ArrayAccess() noexcept {
	}

	void ArrayAccess::invoke() const {
		// TODO: m_var(m_indes);
	}

	void ArrayAccess::print(std::ostream & ostream, std::size_t indent) const {
		ostream << std::string(indent, '\t') << "<ArrayAccess>: " << std::endl;
		m_var->print(ostream, indent + 1);
		m_index->print(ostream, indent + 1);
	}

	UnaryOperation::~UnaryOperation() noexcept {
	}

	void UnaryOperation::invoke() const {
		// TODO: m_op m_arg;
	}

	void UnaryOperation::print(std::ostream & ostream, std::size_t indent) const {
		ostream << std::string(indent, '\t') << "<UnaryOperation>: @" << m_op << std::endl;
		m_rhs->print(ostream, indent + 1);
	}

	BinaryOperation::~BinaryOperation() noexcept {
	}

	void BinaryOperation::invoke() const {
		// TODO: m_lhs m_op m_rhs
	}

	void BinaryOperation::print(std::ostream & ostream, std::size_t indent) const {
		ostream << std::string(indent, '\t') << "<BinaryOperation>: $" << m_op << std::endl;
		m_lhs->print(ostream, indent + 1);
		m_rhs->print(ostream, indent + 1);
	}

	Statement::~Statement() noexcept {
	}

	EmptyStatement::~EmptyStatement() noexcept {
	}

	void EmptyStatement::invoke() const {
	}

	void EmptyStatement::print(std::ostream & ostream, std::size_t indent) const {
		ostream << std::string(indent, '\t') << "<EmptyStatement>" << std::endl;
	}

	CompoundStatement::~CompoundStatement() noexcept {
	}

	void CompoundStatement::invoke() const {
		for (auto const & statement : *m_stmts) {
			statement->invoke();
		}
	}

	void CompoundStatement::print(std::ostream & ostream, std::size_t indent) const {
		ostream << std::string(indent, '\t') << "<CompoundStatement>" << std::endl;
		for (auto const & statement : *m_stmts) {
			statement->print(ostream, indent + 1);
		}
	}

	Program::~Program() noexcept {
	}

	void Program::invoke() const {
		for (auto const & statement : *m_stmts) {
			statement->invoke();
		}
	}

	void Program::print(std::ostream & ostream, std::size_t indent) const {
		ostream << std::string(indent, '\t') << "<Program>" << std::endl;
		for (auto const & statement : *m_stmts) {
			statement->print(ostream, indent + 1);
		}
	}

	ConditionalStatement::~ConditionalStatement() noexcept {
	}

	void ConditionalStatement::invoke() const {
		// TODO: if (m_cond) { m_when_true } else { m_when_false }
	}

	void ConditionalStatement::print(std::ostream & ostream, std::size_t indent) const {
		ostream << std::string(indent, '\t') << "<ConditionalStatement>" << std::endl;
		m_cond->print(ostream, indent + 1);
		m_when_true->print(ostream, indent + 1);
		m_when_false->print(ostream, indent + 1);
	}

	WhileStatement::~WhileStatement() noexcept {
	}

	void WhileStatement::invoke() const {
		// TODO: while (m_cond) { m_body }
	}

	void WhileStatement::print(std::ostream & ostream, std::size_t indent) const {
		ostream << std::string(indent, '\t') << "<WhileStatement>" << std::endl;
		m_cond->print(ostream, indent + 1);
		m_body->print(ostream, indent + 1);
	}

	UntilStatement::~UntilStatement() noexcept {
	}

	void UntilStatement::invoke() const {
		// TODO: until (m_cond) { m_body }
	}

	void UntilStatement::print(std::ostream & ostream, std::size_t indent) const {
		ostream << std::string(indent, '\t') << "<UntilStatement>" << std::endl;
		m_cond->print(ostream, indent + 1);
		m_body->print(ostream, indent + 1);
	}

	ForStatement::~ForStatement() noexcept {
	}

	void ForStatement::invoke() const {
		// TODO
	}

	void ForStatement::print(std::ostream & ostream, std::size_t indent) const {
		ostream << std::string(indent, '\t') << "<ForStatement>: " << m_id << std::endl;
		m_from->print(ostream, indent + 1);
		m_to->print(ostream, indent + 1);
		m_step->print(ostream, indent + 1);
		m_body->print(ostream, indent + 1);
	}

	AssignStatement::~AssignStatement() noexcept {
	}

	void AssignStatement::invoke() const {
		// TODO: m_id = m_expr
	}

	void AssignStatement::print(std::ostream & ostream, std::size_t indent) const {
		ostream << std::string(indent, '\t') << "<AssignStatement>: " << m_id << std::endl;
		m_expr->print(ostream, indent + 1);
	}

	ExpressionStatement::~ExpressionStatement() noexcept {
	}

	void ExpressionStatement::invoke() const {
		// TODO: m_expr
	}

	void ExpressionStatement::print(std::ostream & ostream, std::size_t indent) const {
		ostream << std::string(indent, '\t') << "<ExpressionStatement>: " << std::endl;
		m_expr->print(ostream, indent + 1);
	}

} // namespace br