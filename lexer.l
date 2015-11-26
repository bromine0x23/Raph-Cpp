%option noyywrap nounput batch debug noinput

%{
#include <cerrno>
#include <cstdio>
#include <cstdlib>
#include "../raph.hpp"
#include "parser.hpp"
#include "../location.hpp"

static br::Location location;

#define YY_USER_ACTION location.columns(yyleng);
%}

blank       [ \t\v\f\r]+
comment     ("--"|"//"|\#).*$

lower       [a-z]
upper       [A-Z]
digit       [0-9]
letter      {lower}|{upper}|{digit}|[_]
variable    ({lower}|[_]){letter}*
constant    ({upper}|[_]){letter}*

sign        [+-]
dec         [0-9]
hex         [0-9A-Fa-f]
dec_lead    {sign}?
hex_lead    {sign}?0[xX]
dec_base    {dec}+(\.{dec}+)?
hex_base    {hex}+(\.{hex}+)?
dec_exp     [eE]?{sign}?{dec}+
hex_exp     [pP]?{sign}?{dec}+
dec_numeric {dec_lead}{dec_base}{dec_exp}?
hex_numeric {hex_lead}{hex_base}{hex_exp}?

%%

%{
	location.step();
%}

{blank} {
	location.step();
}

{comment} {
	location.step();
}

"\n"+ {
	location.lines(yyleng);
	return br::Parser::make_SYMBOL_NEWLINE(location);
}

"(" {
	return br::Parser::make_SYMBOL_LPAREN(location);
}

")" {
	return br::Parser::make_SYMBOL_RPAREN(location);
}

"[" {
	return br::Parser::make_SYMBOL_LSQUARE(location);
}

"]" {
	return br::Parser::make_SYMBOL_RSQUARE(location);
}

"," {
	return br::Parser::make_SYMBOL_COMMA(location);
}

";" {
	return br::Parser::make_SYMBOL_SEMICOLON(location);
}

"||" {
	return br::Parser::make_SYMBOL_OR(location);
}

"&&" {
	return br::Parser::make_SYMBOL_AND(location);
}

"==" {
	return br::Parser::make_SYMBOL_EQL(location);
}

"=" {
	return br::Parser::make_SYMBOL_ASSIGN(location);
}

"!=" {
	return br::Parser::make_SYMBOL_NEQ(location);
}

"!" {
	return br::Parser::make_SYMBOL_LOGICAL_NOT(location);
}

"<=" {
	return br::Parser::make_SYMBOL_LE(location);
}

"<" {
	return br::Parser::make_SYMBOL_LT(location);
}

">=" {
	return br::Parser::make_SYMBOL_GE(location);
}

">" {
	return br::Parser::make_SYMBOL_GT(location);
}

"+" {
	return br::Parser::make_SYMBOL_PLUS(location);
}

"-" {
	return br::Parser::make_SYMBOL_MINUS(location);
}

"**" {
	return br::Parser::make_SYMBOL_POW(location);
}

"*" {
	return br::Parser::make_SYMBOL_ASTERISK(location);
}

"/" {
	return br::Parser::make_SYMBOL_SLASH(location);
}

"%" {
	return br::Parser::make_SYMBOL_PERSENT(location);
}

"begin" {
	return br::Parser::make_KEYWORD_BEGIN(location);
}

"end" {
	return br::Parser::make_KEYWORD_END(location);
}

"if" {
	return br::Parser::make_KEYWORD_IF(location);
}

"unless" {
	return br::Parser::make_KEYWORD_UNLESS(location);
}

"then" {
	return br::Parser::make_KEYWORD_THEN(location);
}

"else" {
	return br::Parser::make_KEYWORD_ELSE(location);
}

"while" {
	return br::Parser::make_KEYWORD_WHILE(location);
}

"until" {
	return br::Parser::make_KEYWORD_UNTIL(location);
}

"do" {
	return br::Parser::make_KEYWORD_DO(location);
}

"for" {
	return br::Parser::make_KEYWORD_FOR(location);
}

"from" {
	return br::Parser::make_KEYWORD_FROM(location);
}

"to" {
	return br::Parser::make_KEYWORD_TO(location);
}

"step" {
	return br::Parser::make_KEYWORD_STEP(location);
}

"true" {
	return br::Parser::make_KEYWORD_TRUE(location);
}

"false" {
	return br::Parser::make_KEYWORD_FALSE(location);
}

{variable} {
	return br::Parser::make_TOKEN_VARIABLE(yytext, location);
}

{dec_numeric}|{hex_numeric} {
	return br::Parser::make_TOKEN_NUMERIC(std::stod(yytext), location);
}

. {
	raph.error(location, "Invalid character");
}

<<EOF>> {
	return br::Parser::make_END_OF_FILE(location);
}

%%

namespace br {

	void Raph::scan_begin() {
		yy_flex_debug = trace_scanning;
		if (filename.empty() || filename == "-") {
			yyin = stdin;
		} else if (!(yyin = fopen(filename.c_str(), "r"))) {
			error("cannot open " + filename + ": " + strerror(errno));
			exit(EXIT_FAILURE);
		}
	}

	void Raph::scan_end() {
		fclose(yyin);
	}

}