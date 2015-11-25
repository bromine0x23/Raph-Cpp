Raph
====

绘图语言

编译依赖
----
* CMake
* Flex
* Bison

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
		= "begin" , compound-statement "end"
		| "if"     , expression , then , compound-statement , [ "else" , compound-statement ] , "end"
		| "unless" , expression , then , compound-statement , [ "else" , compound-statement ] , "end"
		| "while" , expression , do , compound-statement , "end"
		| "until" , expression , do , compound-statement , "end"
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
	primary-expression
		= variable
		| constant
		| numeric-literal
		| boolean-literal
		| "(" , expression , "," , expression , ")"
		| function , "(",  [ expression, { "," , expression } ] , ")"
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
		= ( lowercase-letter { identifier-letter } ) - function
		;
	constant
		= ( uppercase-letter { identifier-letter } )
		;
	function
		= (* implementation dependent *)
	sign
		= "+" | "-"
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
	hex-digits
		= hex-digit-letter , { hex-digit-letter }
	dex-numeric
		= [ sign-letter ] ,        dec-digits , [ "." , dec-digits ] , [ [ "e" | "E" ] , [ sign-letter ] , dec-digits ]
	hex-numeric
		= [ sign-letter ] , "0x" , hex-digits , [ "." , hex-digits ] , [ [ "p" | "P" ] , [ sign-letter ] , hex-digits ]
	numeric-literal
		= dex-numeric | hex-numeric
		;
	boolean-literal
		= "true" | "false"
		;
	delimiter
		= ";" | "/n"
		;