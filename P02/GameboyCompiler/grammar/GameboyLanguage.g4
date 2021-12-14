grammar GameboyLanguage;

program: statementList;
statementList: (statement ('\n' | ';' | ';\n'))+;
// Statement: Does not have any return value, can be executed
statement: expression | assignment;

assignment: ID EQ expression;

// Expression: Can be evaluated to a result with type and value (can be empty/void)
expression: term | term PLUS term | term MINUS term | term DIV term | term TIMES term;
term: literal | ID | LPAREN expression RPAREN | call;
literal: NUMBER+;
call: ID LPAREN parameterList? RPAREN;
parameterList: expression | expression COMMA parameterList;


ID: [a-zA-Z][a-zA-Z0-9]*;
NUMBER: [0-9]+;
EQ: '=';
PLUS: '+';
MINUS: '-';
TIMES: '*';
DIV: '/';
POW: '^';
COMMA: ',';
LPAREN: '(';
RPAREN: ')';
WS: ' ' -> skip;