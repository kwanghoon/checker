#include "Lexer.h"

int lexer(char buf[], int buf_len, int tokens[], int toks_size)
{
	int cur_buf;
	int cur_tok;
	int error;

	cur_buf = 0;
	cur_tok = 0;
	error   = 0;

	while (cur_buf < buf_len && cur_tok < toks_size && error == 0)
	{
		switch( buf[cur_buf] )
		{
		case ':':
			tokens[cur_tok] = COLON;
			cur_buf++;
			cur_tok++;
			break;

		case ';':
			while (cur_buf < buf_len && buf[cur_buf] != '\n')
				cur_buf++;
			break;

		case '.':
			tokens[cur_tok] = DOT;
			cur_buf++;
			cur_tok++;
			break;

		case '(':
			tokens[cur_tok] = OPEN;
			cur_buf++;
			cur_tok++;
			break;

		case ')':
			tokens[cur_tok] = CLOSE;
			cur_buf++;
			cur_tok++;
			break;

		case '-':
			if (cur_buf+1 < buf_len && buf[cur_buf+1]=='>')
			{
				tokens[cur_tok] = ARROW;
				cur_buf += 2;
				cur_tok++;
			}
			else
			{
				error = -1; // Not ->
			}
			break;

		case '/':
			if (cur_buf+1 < buf_len && buf[cur_buf+1]=='\\')
			{
				tokens[cur_tok] = PI;
				cur_buf += 2;
				cur_tok++;
			}
			else
			{
				error = -2; // Not LAM
			}
			break;

		case '\\':
			tokens[cur_tok] = LAM;
			cur_buf++;
			cur_tok++;
			break;

		case '@':
			// type
			if (cur_buf+4 < buf_len
				&& buf[cur_buf+1]=='t'
				&& buf[cur_buf+2]=='y'
				&& buf[cur_buf+3]=='p'
				&& buf[cur_buf+4]=='e')
			{
				tokens[cur_tok] = ATTYPE;
				cur_buf += 5;
				cur_tok++;
			}
			else if(cur_buf+4 < buf_len
				&& buf[cur_buf+1]=='t'
				&& buf[cur_buf+2]=='e'
				&& buf[cur_buf+3]=='r'
				&& buf[cur_buf+4]=='m')
			{
				tokens[cur_tok] = ATTERM;
				cur_buf += 5;
				cur_tok++;
			}
			else if(cur_buf+3 < buf_len
				&& buf[cur_buf+1]=='d'
				&& buf[cur_buf+2]=='e'
				&& buf[cur_buf+3]=='f')
			{
				tokens[cur_tok] = ATDEF;
				cur_buf += 4;
				cur_tok++;
			}
			else 
			{
				error = -3; // Not @type, @term, @def
			}
			break;
			
		case '=':
			tokens[cur_tok] = EQ;
			cur_buf++;
			cur_tok++;
			break;

		case ' ':
		case '\t':
		case '\n':
			cur_buf++;
			break;

		default:
			if ('a' <= buf[cur_buf] && buf[cur_buf] <= 'z'
				|| 'A' <= buf[cur_buf] && buf[cur_buf] <= 'Z')
			{
				int start;

				start = cur_buf;
				while( cur_buf < buf_len &&
						(  'a' <= buf[cur_buf] && buf[cur_buf] <= 'z'
						|| 'A' <= buf[cur_buf] && buf[cur_buf] <= 'Z'
						|| '0' <= buf[cur_buf] && buf[cur_buf] <= '9'
						|| buf[cur_buf] == '\'' 
						|| buf[cur_buf] == '_' 
						)
					) cur_buf++;

				if ( cur_buf-start == 4 &&			// Token
						(  buf[start+0] == 'T'
						&& buf[start+1] == 'y'
						&& buf[start+2] == 'p'
						&& buf[start+3] == 'e' ) ) {

					tokens[cur_tok] = TYPE;
					cur_tok++;
				}
				else if ( cur_tok+3 < toks_size ) {
					tokens[cur_tok] = VAR;				// Var token header
					cur_tok++;

					tokens[cur_tok] = start;			// The starting position
					cur_tok++;

					tokens[cur_tok] = cur_buf-start;	// The length
					cur_tok++;
				}
				else 
					error = -4;	// Var is recognized but the tokens buffer is too small to keep it.
			}
			else 
			{
				error = -5; // Var name fails to be recognized.
			}
			break;
		}
	}

	if (error == 0) {
		tokens[cur_tok] = ENDOFSYMBOL;
		cur_tok++;
		return cur_tok;
	}
	else
		return error;
}
