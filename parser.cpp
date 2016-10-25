#include "parser.h"

Parser::ParserResult
Parser::parse( std::string e_ ){
	exp = e_; // Guarda expressão do parametro
	curr_symb = exp.begin(); // caractere inicial

	//expression();

	return curr_status;
}

Parser::terminal_symbol_t
Parser::lexer( char c_ ) const{
	switch( c_ ){
		case '(': return TS_L_PAREN;
		case '+': return TS_PLUS;
		case '-': return TS_MINUS;
		case ')': return TS_R_PAREN;
		case ' ': return TS_WS;
		case   9: return TS_TAB;
		case '0': return TS_ZERO;
		case 1:
		case 2:
		case 3:
		case 4:
		case 5:
		case 6:
		case 7:
		case 8:
		case 9: return TS_NON_ZERO_DIGIT;
	}
}

bool
Parser::peek( terminal_symbol_t s_ ){
	return lexer( *curr_symb ) == s_;
}

bool
Parser::end_input( void ) const{
	return curr_symb == exp.end();
}

void
Parser::skip_ws( void ){
	while( not end_input() and ( peek( TS_WS ) or peek( TS_TAB ) ) ){
		next_symbol();
	}
}

bool
Parser::accept( terminal_symbol_t s_ ){
	if( not end_input() and peek( s_ ) ){
		next_symbol();
		return true;
	}
	return false;
}

bool
Parser::expect( terminal_symbol_t s_ ){
	// 1) saltar espaços em brancos
	skip_ws();
	// 2) tenta aceitar o simbolo
	return accept( s_ );
}

void
Parser::next_symbol( void ){
	std::advance( curr_symbol, 1 );
}
