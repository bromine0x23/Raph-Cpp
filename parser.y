%code requires{
#include <string>
#include <memory>
#include <list>
#include "../ast.hpp"
#include "../location.hpp"
#include "../raph.hpp"
#define YY_NULLPTR nullptr
#define YY_DECL int yylex(br::Parser::semantic_type * yylval, br::Parser::location_type * yylloc, void * yyscanner, br::RaphParser const & parser)
}
%code provides{
extern "C" {
	YY_DECL;
	using YYScan = void *;
	using YYSize = std::size_t;
#define YY_EXTRA_TYPE std::nullptr_t
	auto yyalloc(YYSize size, YYScan scanner) -> void *;
	auto yyrealloc(void * ptr, YYSize size, YYScan scanner) -> void *;
	void yyfree(void *, YYScan scanner);
	auto yylex_init(YYScan * scanner) -> int;
	auto yylex_init_extra(YY_EXTRA_TYPE, YYScan * scanner) -> int;
	auto yylex_destroy(YYScan scanner) -> int;
	auto yyget_debug(YYScan scanner) -> int;
	void yyset_debug(int debug_flag, YYScan scanner);
	auto yyget_extra(YYScan scanner) -> YY_EXTRA_TYPE;
	void yyset_extra(YY_EXTRA_TYPE,YYScan scanner);
	auto yyget_in(YYScan scanner) -> FILE *;
	void yyset_in(FILE * in_str, YYScan scanner);
	auto yyget_out(YYScan scanner) -> FILE *;
	void yyset_out(FILE * out_str, YYScan scanner);
	auto yyget_leng(YYScan scanner) -> YYSize;
	auto yyget_text(YYScan scanner) -> char *;
	auto yyget_lineno(YYScan scanner) -> int;
	void yyset_lineno(int line_number, YYScan scanner);
	auto yyget_column(YYScan scanner) -> int;
	void yyset_column(int column_no, YYScan scanner);
} // extern "C"
}
%define api.namespace {br}
%define api.value.type variant
%define api.location.type {br::Location}
%define parser_class_name { Parser }
%define parse.assert
%define parse.trace
%locations
%param { void * yyscanner }
%param { RaphParser const & parser }
%parse-param { std::shared_ptr<Program> & program }
%initial-action {
	@$.begin.filename = @$.end.filename = std::make_shared<std::string>(parser.filename());
}
%token
	END_OF_FILE         0 "end of file"
	SYMBOL_NEWLINE     10 "\n"
	SYMBOL_LOGICAL_NOT 33 "!"
	SYMBOL_PERSENT     37 "%"
	SYMBOL_LPAREN      40 "("
	SYMBOL_RPAREN      41 ")"
	SYMBOL_ASTERISK    42 "*"
	SYMBOL_PLUS        43 "+"
	SYMBOL_COMMA       44 ","
	SYMBOL_MINUS       45 "-"
	SYMBOL_SLASH       47 "/"
	SYMBOL_SEMICOLON   59 ";"
	SYMBOL_LT          60 "<"
	SYMBOL_ASSIGN      61 "="
	SYMBOL_GT          62 ">"
	SYMBOL_LSQUARE     91 "["
	SYMBOL_RSQUARE     93 "]"
	SYMBOL_OR             "||"
	SYMBOL_AND            "&&"
	SYMBOL_EQL            "=="
	SYMBOL_NEQ            "!="
	SYMBOL_LE             "<="
	SYMBOL_GE             ">="
	SYMBOL_POW            "**"
%token
	KEYWORD_BEGIN  "begin"
	KEYWORD_END    "end"
	KEYWORD_IF     "if"
	KEYWORD_UNLESS "unless"
	KEYWORD_THEN   "then"
	KEYWORD_ELSE   "else"
	KEYWORD_WHILE  "while"
	KEYWORD_UNTIL  "until"
	KEYWORD_DO     "do"
	KEYWORD_FOR    "for"
	KEYWORD_FROM   "from"
	KEYWORD_TO     "to"
	KEYWORD_STEP   "step"
	KEYWORD_TRUE   "true"
	KEYWORD_FALSE  "false"
%token <double>
	TOKEN_NUMERIC
%token <std::string>
	TOKEN_VARIABLE
	TOKEN_CONSTANT
%type < std::shared_ptr<Program> >
	program
%type < std::shared_ptr< std::list< std::shared_ptr<Statement> > > >
	statements
%type < std::shared_ptr<Statement> >
	statement
	compound-statement
	optional-else
%type < std::shared_ptr<Expression> >
	expression
	logical-or-expression
	logical-and-expression
	equality-expression
	relation-expression
	additive-expression
	multiplicative-expression
	power-expression
	unary-expression
	postfix-expression
	primary-expression
%type < std::shared_ptr< std::list< std::shared_ptr<Expression> > > >
	optional-arguments
	arguments
%type <bool>
	boolean-literal
%start program
%%
program
	: statements optional-delimiters
		{
			program = $[program] = std::make_shared<Program>($[statements]);
		}
	;
statements
	: %empty
		{
			$[statements] = std::make_shared< std::list< std::shared_ptr<Statement> > >();
		}
	| statement
		{
			$[statements] = std::make_shared< std::list< std::shared_ptr<Statement> > >();
			$[statements]->push_back($[statement]);
		}
	| statements[current] delimiters statement
		{
			$$ = $[current];
			$$->push_back($[statement]);
		}
	| error statement
		{
			$[statements] = std::make_shared< std::list< std::shared_ptr<Statement> > >();
		}
	;
compound-statement
	: statements optional-delimiters
		{
			$[compound-statement] = std::make_shared<CompoundStatement>($[statements]);
		}
	;
statement
	: "begin" compound-statement "end"
		{
			$[statement] = $[compound-statement];
		}
	| "if" expression then compound-statement[when-true] optional-else[when-false] "end"
		{
			$[statement] = std::make_shared<ConditionalStatement>($[expression], $[when-true], $[when-false]);
		}
	| "unless" expression then compound-statement[when-false] optional-else[when-true] "end"
		{
			$[statement] = std::make_shared<ConditionalStatement>($[expression], $[when-false], $[when-true]);
		}
	| "while" expression do compound-statement "end"
		{
			$[statement] = std::make_shared<WhileStatement>($[expression], $[compound-statement]);
		}
	| "until" expression do compound-statement "end"
		{
			$[statement] = std::make_shared<UntilStatement>($[expression], $[compound-statement]);
		}
	| "for" TOKEN_VARIABLE[id] "from" expression[from] "to" expression[to] "step" expression[step] compound-statement[body] "end"
		{
			$[statement] = std::make_shared<ForStatement>($[id], $[from], $[to], $[step], $[body]);
		}
	| TOKEN_VARIABLE[id] "=" expression
		{
			$[statement] = std::make_shared<AssignStatement>($[id], $[expression]);
		}
	| expression
		{
			$[statement] = std::make_shared<ExpressionStatement>($[expression]);
		}
	;
then
	: delimiter
	| "then"
	| delimiter "then"
	;
do
	: delimiter
	| "do"
	| delimiter "do"
	;
optional-else
	: %empty
		{
			$$ = std::make_shared<EmptyStatement>();
		}
	| "else" compound-statement
		{
			$[optional-else] = $[compound-statement];
		}
	;
expression
	: logical-or-expression { $$ = $1; }
	;
logical-or-expression[res]
	: logical-and-expression { $$ = $1; }
	| logical-or-expression[lhs] "||" logical-and-expression[rhs]
		{
			$[res] = std::make_shared<BinaryOperation>("||", $[lhs], $[rhs]);
		}
	;
logical-and-expression[res]
	: equality-expression { $$ = $1; }
	| logical-and-expression[lhs] "&&" equality-expression[rhs]
		{
			$[res] = std::make_shared<BinaryOperation>("&&", $[lhs], $[rhs]);
		}
	;
equality-expression[res]
	: relation-expression { $$ = $1; }
	| relation-expression[lhs] "==" relation-expression[rhs]
		{
			$[res] = std::make_shared<BinaryOperation>("==", $[lhs], $[rhs]);
		}
	| relation-expression[lhs] "!=" relation-expression[rhs]
		{
			$[res] = std::make_shared<BinaryOperation>("!=", $[lhs], $[rhs]);
		}
relation-expression[res]
	: additive-expression { $$ = $1; }
	| relation-expression[lhs] "<" additive-expression[rhs]
		{
			$[res] = std::make_shared<BinaryOperation>("<", $[lhs], $[rhs]);
		}
	| relation-expression[lhs] ">" additive-expression[rhs]
		{
			$[res] = std::make_shared<BinaryOperation>(">", $[lhs], $[rhs]);
		}
	| relation-expression[lhs] "<=" additive-expression[rhs]
		{
			$[res] = std::make_shared<BinaryOperation>("<=", $[lhs], $[rhs]);
		}
	| relation-expression[lhs] ">=" additive-expression[rhs]
		{
			$[res] = std::make_shared<BinaryOperation>(">=", $[lhs], $[rhs]);
		}
	;
additive-expression[res]
	: multiplicative-expression { $$ = $1; }
	| additive-expression[lhs] "+" multiplicative-expression[rhs]
		{
			$[res] = std::make_shared<BinaryOperation>("+", $[lhs], $[rhs]);
		}
	| additive-expression[lhs] "-" multiplicative-expression[rhs]
		{
			$[res] = std::make_shared<BinaryOperation>("-", $[lhs], $[rhs]);
		}
	;
multiplicative-expression[res]
	: power-expression { $$ = $1; }
	| multiplicative-expression[lhs] "*" power-expression[rhs]
		{
			$[res] = std::make_shared<BinaryOperation>("*", $[lhs], $[rhs]);
		}
	| multiplicative-expression[lhs] "/" power-expression[rhs]
		{
			$[res] = std::make_shared<BinaryOperation>("/", $[lhs], $[rhs]);
		}
	| multiplicative-expression[lhs] "%" power-expression[rhs]
		{
			$[res] = std::make_shared<BinaryOperation>("%", $[lhs], $[rhs]);
		}
	;
power-expression[res]
	: unary-expression { $$ = $1; }
	| unary-expression[lhs] "**" power-expression[rhs]
		{
			$[res] = std::make_shared<BinaryOperation>("**", $[lhs], $[rhs]);
		}
	;
unary-expression[res]
	: postfix-expression { $$ = $1; }
	| "!" unary-expression[rhs]
		{
			$[res] = std::make_shared<UnaryOperation>("!", $[rhs]);
		}
	| "+" unary-expression[rhs]
		{
			$[res] = std::make_shared<UnaryOperation>("+", $[rhs]);
		}
	| "-" unary-expression[rhs]
		{
			$[res] = std::make_shared<UnaryOperation>("-", $[rhs]);
		}
	;
postfix-expression[expr]
	: primary-expression { $$ = $1; }
	| postfix-expression[func] "(" optional-arguments[args] ")"
		{
			$[expr] = std::make_shared<FunctionCall>($[func], $[args]);
		}
	| postfix-expression[var] "[" expression[index] "]"
		{
			$[expr] = std::make_shared<ArrayAccess>($[var], $[index]);
		}
	;
primary-expression[expr]
	: TOKEN_VARIABLE[id]
		{
			$[expr] = std::make_shared<Variable>($[id]);
		}
	| TOKEN_CONSTANT[id]
		{
			$[expr] = std::make_shared<Constant>($[id]);
		}
	| TOKEN_NUMERIC[value]
		{
			$[expr] = std::make_shared<Numeric>($[value]);
		}
	| boolean-literal[value]
		{
			$[expr] = std::make_shared<Boolean>($[value]);
		}
	| "(" expression[x] "," expression[y] ")"
		{
			$[expr] = std::make_shared<Vector>($[x], $[y]);
		}
	;
boolean-literal
	: "true"  { $$ = true; }
	| "false" { $$ = false; }
	;
optional-arguments[arguments]
	: %empty
		{
			$[arguments] = std::make_shared< std::list< std::shared_ptr<Expression> > >();
		}
	| arguments { $$ = $1; }
	;
arguments
	: expression[argument]
		{
			$[arguments] = std::make_shared< std::list< std::shared_ptr<Expression> > >(std::list< std::shared_ptr<Expression> >{$[argument]});
		}
	| arguments[current] "," expression
		{
			$$ = $[current];
			$$->push_back($[expression]);
		}
	;
optional-delimiters
	: %empty
	| delimiters
	;
delimiter
	: ";"
	| "\n"
	;
delimiters
	: delimiter
	| delimiters delimiter
	;
%%
void br::Parser::error(location_type const & location, std::string const & message) {
	parser.error(location, message);
}