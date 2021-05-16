# CS 280 Language Parser and Interpreter

This project is the culmination of building a language lexical analyzer, parser, and interpreter for Spring 2020 CS 280 which was taught by the late Professor Gerard Ryan. This is for archival purposes and will not be useful for any students currently taking CS 280.

## Language

Prog := Sl  
Sl := SC { Sl } | Stmt SC { Sl }  
Stmt := PrintStmt | PrintlnStmt | RepeatStmt | Expr PrintStmt := PRINT Expr  
PrintlnStmt := PRINTLN Expr  
RepeatStmt:= Repeat Expr BEGIN Stmt END  
Expr := Sum { EQ Sum }  
Sum := Prod { (PLUS | MINUS) Prod }  
Prod := Primary { (STAR | SLASH) Primary }  
Primary := IDENT | ICONST | SCONST | LPAREN Expr RPAREN  

Rules:
1. The language has identifiers, which are defined to be a letter followed by zero or more
letters or numbers. This will be the Token IDENT.
2. The language has integer constants, which are defined to be one or more digits with an
optional leading sign. This will be the Token ICONST.
3. The language has string constants, which are a double-quoted sequence of characters,
all on the same line. This will be the Token SCONST.
4. A string constant can include escape sequences: a backslash followed by a character.
The sequence \n should be interpreted as a newline. The sequence \ should be interpreted as a backslash. All other escapes should simply be interpreted as the character after the backslash.
5. The language has reserved the keywords print, println, repeat, begin, end. They will be Tokens PRINT PRINTLN REPEAT BEGIN END.
6. The language has several operators. They are + - * = / ( ) which will be Tokens PLUS MINUS STAR SLASH EQ LPAREN RPAREN
7. The language recognizes a semicolon as the token SC
8. A comment is all characters from // to the end of the line; it is ignored and is not returned
as a token. NOTE that a // in the middle of a SCONST is NOT a comment!
9. Whitespace between tokens can be used for readability, and can serve as one way to
delimit tokens.
10. An error will be denoted by the ERR token.
11. End of file will be denoted by the DONE token.

## Programs
**Programming Assignment 2 - Lexical Analyzer**: analyzer.cpp  
Parameters:
- -v (optional) if present, every token is printed when it is seen
- -iconsts (optional) if present, print out all the unique integer constants in
alphabetical order
- -sconsts (optional) if present, prints out all the unique integer constants in
numeric order
- -ids (optional) if present, print out all of the unique identifiers in alphabetical order

**Programming Assignment 3 - Recursive Descent Parser**: descentParser.cpp

**Programming Assignment 4 - Interpreter**: interpreter.cpp  
Rules:
1. The language contains two types: integer and string.
2. The PLUS MINUS STAR and SLASH operators are left associative.
3. The EQ operator is right associative.
4. A Repeat statement evaluates the Expr. The Expr must evaluate to an integer ​[RT]​. If the integer is nonzero, then the Stmt is executed, otherwise it is not. The value of the Expr is decremented by 1 each repetition. In other words “repeat 5 begin println “hello” end” will print “hello” 5 times.
5. A PrintStmt evaluates the Expr and prints its value.
6. A PrintlnStmt evaluates the Expr and prints its value, followed by a newline character.
7. The type of an IDENT is the type of the value assigned to it.
8. The EQ operator assigns values to variables. It evaluates the Expr on the right hand side
and saves its value in memory associated with the left hand side (an IDENT). If the
IDENT does not exist, it is created. If the IDENT already exists, its value is replaced.
9. The PLUS and MINUS operators in Expr represent addition and subtraction.
10. The STAR and SLASH operators in Prod represent multiplication and division.
11. It is an error if a variable is used before a value is assigned to it ​[RT]​.
12. Addition is defined between two integers (the result being the sum) or two strings (the result being the concatenation).
13. Subtraction is defined between two integers (the result being the difference) or two strings (the result being removal of the first instance of the second string from the first string, if any).
14. Multiplication is defined between two integers (the result being the product) or for an integer and a string (the result being the string repeated integer times).
15. Division is defined between two integers.
16. Performing an operation with incorrect types or type combinations is an error ​[RT]​.
17. Multiplying a string by a negative integer is an error ​[RT]​.
18. A Repeat statement whose Expr is not integer typed is an error ​[RT]​.

*[RT] = checked at runtime*
