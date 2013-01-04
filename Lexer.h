
#ifndef LEXER_H
#define LEXER_H

enum Token {
	TYPE=1, PI, LAM, COLON, DOT,
	OPEN, CLOSE, EQ, ARROW, ATTYPE,
	ATTERM, ATDEF, VAR, NUM,

        /* Needed in the parser, not int the lexer */
	ENDOFSYMBOL, 
	NONTERMINAL  
};

int lexer(char buf[], int buf_size, int tokens[], int toks_size);

#endif // LEXER_H
