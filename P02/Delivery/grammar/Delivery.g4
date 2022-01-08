grammar Delivery;

/*
 * Parser Rules
 */

delivery: order*;
order: name COMMA dish COMMA price;

name: ID ID;
dish: ID;
price: FLOAT;

/*
 * Lexer Rules
 */

NUMBER : DIGIT+;
ID : LETTER+;
FLOAT : DIGIT+ DOT DIGIT DIGIT;
DIGIT: [0-9];
LETTER: [a-zA-Z];
DOT: '.';
COMMA: ',';
WHITESPACE: [ \t\r\n] -> skip;
