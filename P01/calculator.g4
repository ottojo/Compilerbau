grammar calculator;

s: expression;
expression: term PLUS expression | term MINUS expression | term;
term: factor TIMES term | factor;
factor: unit |  unit POW unit | unit DIV unit;
unit: literal | LPAREN expression RPAREN;
literal: MINUS NUM | NUM;

NUM : [0-9]+;
PLUS: '+';
MINUS: '-';
TIMES: '*';
DIV: '/';
POW: '^';
LPAREN: '(';
RPAREN: ')';
WS: [ \n\t\r]+ -> skip;
