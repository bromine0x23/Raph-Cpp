Raph
====

绘图语言

编译依赖
----
* CMake 3.3
* Flex 2.5
* Bison 3.0
* Boost 1.59

语法
----
	program
		= statements , { delimiter }
		;
	statements
		= [ statement , { { delimiter } statement } ]
		;
	compound-statement
		= statements { delimiter }
		;
	statement
		= "begin" , compound-statement , "end"
		| ( "if" | "unless" ) , expression , then , compound-statement , [ "else" , compound-statement ] , "end"
		| ( "while" | "until" ) , expression , do , compound-statement , "end"
		| "for" , variable , "from" , expression , "to" , expression , "step" , expression , compound-statement , "end"
		| variable , "=" , expression
		| expression
		;
	then 
		= delimiter | "then" | delimiter , "then"
		;
	do
		= delimiter | "do" | delimiter , "do"
		;
	expression
		= logical-or-expression
		;
	logical-or-expression
		= logical-and-expression , { "||" , logical-and-expression }
		;
	logical-and-expression
		= equality-expression , { "&&" , equality-expression }
		;
	equality-expression
		= relation-expression , [ ( "==" | "!=" ) , relation-expression ]
		;
	relation-expression
		= additive-expression , { ( "<" | ">" | "<=" | ">=" ) , additive-expression }
		;
	additive-expression
		= multiplicative-expression , { ( "+" | "-" ) , multiplicative-expression }
		;
	multiplicative-expression
		= power-expression , { ( "*" | "/" | "%" ) , power-expression }
		;
	power-expression
		= { unary-expression ,  "**" } , unary-expression
		;
	unary-expression
		= [ "!" | "+" | "-" ] primary-expression
		;
	postfix-expression
		= primary-expression ,
			{
				( "(" , [ expression, { "," , expression } ] , ")" )
			|
				( "[" ,   expression                         , "]" )
			}
		;
	primary-expression
		= variable
		| constant
		| numeric-literal
		| boolean-literal
		| "(" , expression , "," , expression , ")"
		;
	lowercase-letter
		= "a" | "b" | "c" | "d" | "e" | "f" | "g"
		| "h" | "i" | "j" | "k" | "l" | "m" | "n"
		| "o" | "p" | "q" | "r" | "s" | "t" | "u"
		| "v" | "w" | "x" | "y" | "z"
		;
	uppercase-letter
		= "A" | "B" | "C" | "D" | "E" | "F" | "G"
		| "H" | "I" | "J" | "K" | "L" | "M" | "N"
		| "O" | "P" | "Q" | "R" | "S" | "T" | "U"
		| "V" | "W" | "X" | "Y" | "Z"
		;
	digit-letter
		= "0" | "1" | "2" | "3" | "4" | "5" | "6" | "7" | "8" | "9"
		;
	identifier-letter
		= lowercase-letter | uppercase-letter | digit-letter | "_" ;
	variable
		= ( lowercase-letter | "_" ) , { identifier-letter }
		;
	constant
		= ( uppercase-letter | "_" ) , { identifier-letter }
		;
	dec-digit
		= "0" | "1" | "2" | "3" | "4" | "5" | "6" | "7" | "8" | "9"
		;
	hex-digit
		= "0" | "1" | "2" | "3" | "4" | "5" | "6" | "7" | "8" | "9"
		| "A" | "B" | "C" | "D" | "E" | "F"
		| "a" | "b" | "c" | "d" | "e" | "f"
		;
	dec-digits
		= dec-digit-letter , { dec-digit-letter }
		;
	hex-digits
		= hex-digit-letter , { hex-digit-letter }
		;
	dex-numeric
		= [ "+" | "-" ] ,        dec-digits , [ "." , dec-digits ] , [ [ "e" | "E" ] , [ "+" | "-" ] , dec-digits ]
		;
	hex-numeric
		= [ "+" | "-" ] , "0x" , hex-digits , [ "." , hex-digits ] , [ [ "p" | "P" ] , [ "+" | "-" ] , hex-digits ]
		;
	numeric-literal
		= dex-numeric | hex-numeric
		;
	boolean-literal
		= "true" | "false"
		;
	delimiter
		= ";" | "/n"
		;