grammar GameboyLanguage;

program: statementList EOF;
statementList: (statement? ('\n' | ';' | ';\n'))+;
// Statement: Does not have any return value, can be executed
statement: expression | assignment | declaration;

assignment: variableName=ID EQ rhs=expression;
declaration: typeName=ID variableName=ID EQ rhs=expression;

// Expression: Can be evaluated to a result with type and value (can be empty/void)
expression: singleTerm=term | lhs=term PLUS rhs=term | lhs=term MINUS rhs=term | lhs=term DIV rhs=term | lhs=term TIMES rhs=term | lhs=term XOR rhs=term;
term: literal | variable=ID | LPAREN expression RPAREN | call;
literal: NUMBER+;
call: functionName=ID LPAREN parameterList? RPAREN;
parameterList: firstExpression=expression | firstExpression=expression COMMA remainingList=parameterList;


ID: [a-zA-Z][a-zA-Z0-9]*;
NUMBER: [0-9]+;
EQ: '=';
PLUS: '+';
MINUS: '-';
TIMES: '*';
DIV: '/';
XOR: '^';
COMMA: ',';
LPAREN: '(';
RPAREN: ')';
WS: [ \t]+ -> skip;
COMMENT: '//'~[\r\n]* -> skip;
