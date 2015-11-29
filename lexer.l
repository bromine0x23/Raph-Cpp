%option batch
%option noyywrap nounput noinput
%option reentrant
%top{
#include <cstddef>
#include <memory>
#include "parser.hpp"
#define YY_USER_ACTION yylloc->columns(yyleng);
}

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
	yylloc->step();
%}
{blank} {
	yylloc->step();
}
{comment} {
	yylloc->step();
}
"\n"+ {
	yylloc->lines(yyleng);
	return br::Parser::token::SYMBOL_NEWLINE;
}
"(" {
	return br::Parser::token::SYMBOL_LPAREN;
}
")" {
	return br::Parser::token::SYMBOL_RPAREN;
}
"[" {
	return br::Parser::token::SYMBOL_LSQUARE;
}
"]" {
	return br::Parser::token::SYMBOL_RSQUARE;
}
"," {
	return br::Parser::token::SYMBOL_COMMA;
}
";" {
	return br::Parser::token::SYMBOL_SEMICOLON;
}
"||" {
	return br::Parser::token::SYMBOL_OR;
}
"&&" {
	return br::Parser::token::SYMBOL_AND;
}
"==" {
	return br::Parser::token::SYMBOL_EQL;
}
"=" {
	return br::Parser::token::SYMBOL_ASSIGN;
}
"!=" {
	return br::Parser::token::SYMBOL_NEQ;
}
"!" {
	return br::Parser::token::SYMBOL_LOGICAL_NOT;
}
"<=" {
	return br::Parser::token::SYMBOL_LE;
}
"<" {
	return br::Parser::token::SYMBOL_LT;
}
">=" {
	return br::Parser::token::SYMBOL_GE;
}
">" {
	return br::Parser::token::SYMBOL_GT;
}
"+" {
	return br::Parser::token::SYMBOL_PLUS;
}
"-" {
	return br::Parser::token::SYMBOL_MINUS;
}
"**" {
	return br::Parser::token::SYMBOL_POW;
}
"*" {
	return br::Parser::token::SYMBOL_ASTERISK;
}
"/" {
	return br::Parser::token::SYMBOL_SLASH;
}
"%" {
	return br::Parser::token::SYMBOL_PERSENT;
}
"begin" {
	return br::Parser::token::KEYWORD_BEGIN;
}
"end" {
	return br::Parser::token::KEYWORD_END;
}
"if" {
	return br::Parser::token::KEYWORD_IF;
}
"unless" {
	return br::Parser::token::KEYWORD_UNLESS;
}
"then" {
	return br::Parser::token::KEYWORD_THEN;
}
"else" {
	return br::Parser::token::KEYWORD_ELSE;
}
"while" {
	return br::Parser::token::KEYWORD_WHILE;
}
"until" {
	return br::Parser::token::KEYWORD_UNTIL;
}
"do" {
	return br::Parser::token::KEYWORD_DO;
}
"for" {
	return br::Parser::token::KEYWORD_FOR;
}
"from" {
	return br::Parser::token::KEYWORD_FROM;
}
"to" {
	return br::Parser::token::KEYWORD_TO;
}
"step" {
	return br::Parser::token::KEYWORD_STEP;
}
"true" {
	return br::Parser::token::KEYWORD_TRUE;
}
"false" {
	return br::Parser::token::KEYWORD_FALSE;
}
{variable} {
	yylval->build<std::string>() = yytext;
	return br::Parser::token::TOKEN_VARIABLE;
}
{constant} {
	yylval->build<std::string>() = yytext;
	return br::Parser::token::TOKEN_CONSTANT;
}
{dec_numeric}|{hex_numeric} {
	yylval->build<double>() = std::stod(yytext);
	return br::Parser::token::TOKEN_NUMERIC;
}
. {
	parser.error(*yylloc, "Invalid character");
}
<<EOF>> {
	return br::Parser::token::END_OF_FILE;
}
%%