grammar GameboyLanguage;

program: ((funcDefinition | builtinFuncDeclaration | globalVarInitialization)? ('\n' | ';' | ';\n'))* EOF;

statementList: (statement? ('\n' | ';' | ';\n'))*;
// Statement: Does not have any return value, can be executed
statement: expression | assignment | varInitialization | returnStatement;

varDeclaration: typeName=ID variableName=ID;
varInitialization: varDeclaration EQ rhs=expression;
globalVarInitialization : varInitialization;
assignment: variableName=ID EQ rhs=expression;

funcSignature:funcName=ID LPAREN argumentList RPAREN ('->' returnType=ID)?;
argumentList: (varDeclaration (COMMA varDeclaration)*)?;
funcDeclaration: 'func' funcSignature;
funcDefinition: funcDeclaration block;
builtinFuncDeclaration: 'builtin' funcDeclaration;

block: LBRACE statementList RBRACE;

returnStatement: 'return' (rhs=expression)?;


// Expression: Can be evaluated to a result with type and value (can be empty/void)
expression: singleTerm=term | lhs=term PLUS rhs=term | lhs=term MINUS rhs=term | lhs=term DIV rhs=term | lhs=term TIMES rhs=term | lhs=term XOR rhs=term;
term: literal | variable=ID | LPAREN expression RPAREN | call;
literal: NUMBER+;
call: functionName=ID LPAREN parameterList? RPAREN;
parameterList: expression (COMMA expression)*;


ID: [a-zA-Z][a-zA-Z0-9_]*;
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
LBRACE: '{';
RBRACE: '}';
WS: [ \t]+ -> skip;
COMMENT: '//'~[\r\n]* -> skip;
