#include "evaluator.h"


bool
Evaluator::has_higher_precedence( Token op1, Token op2 ){
	bool result ( true );
	if( get_operator_precedence( op1 ) == get_operator_precedence( op2 ) ){
		if( right_association( op1 ) )
			result = not result;
	}
	else{
		result = get_operator_precedence( op1 ) > get_operator_precedence( op2 );
	}
	return result;
}

bool 
Evaluator::right_association( Token tk_ ){
	return ( tk_.value == '^' );
}

bool 
Evaluator::is_operator( Token tk_ ){
	return ( tk_.value == '+' or tk_.value == '-' or tk_.value == '/' or tk_.value == '%' or tk_.value == '*' or tk_.value == '^');
}

bool 
Evaluator::is_operand( Token tk_ ){
	return ( tk_.value >= '0' and tk_.value <= '9' );
}

bool 
Evaluator::is_opening_scope( Token tk_ ){
	return tk_.value == '(';
}


bool 
Evaluator::is_closing_scope( Token tk_ ){
	return tk_.value == ')';
}

int 
Evaluator::get_operator_precedence( Token tk_ ){
	int weight = -1;

	switToken( tk_.value ){
		case '^' : weight = 3; break;
		case '*' : 
		case '/' :
		case '%' : weight = 2; break;
		case '+' :
		case '-' : weight = 1; break;
	}

	return weight;
}

void
Evaluator::infix_to_postfix( std::vector< Token > infix ){
	std::vector< Token > postfix;
	std::stack< Token > S;

	// Percorrer cada caractere da expressão
	for( auto Token : infix ){
		// Abertura de escopo.
		if( is_opening_scope( Token ) ){
			S.push( Token );
		}
		else if( is_operand( Token ) ){
			postfix.push_back( Token );
		}
		else if( is_closing_scope( Token ) ){
			// Desempilhar ate aTokenar o escopo de abertura correspondente.
			while( not S.empty() and not is_opening_scope( S.top() ) ){
				postfix.push_back( S.top() );
				S.pop();
			}
			S.pop();
		}
		else if( is_operator( Token ) ){
			// Desempilhar as operações que tem maior (ou igual) prioridade.
			while( not S.empty() and not is_opening_scope( S.top() ) and has_higher_precedence( S.top(), Token ) ){
				postfix.push_back( S.top() );
				S.pop();
			}
			S.push( Token ); // O novo operador sempre entra na pilha.
		}
		// Elemento não esperado.
		else{
			continue;
		}
		// Mandando pra saída
	} //ranged-for
	// Esvaziar a pilha para a saida ( operações pendentes ).
	while( not S.empty() ){
		postfix.push_back( S.top() );
		S.pop();
	}
	return postfix;
}

result_t
Evaluator::char_2_int( Token tk_ ){
	return tk_.value - '0';
}

result_t
Evaluator::apply_operation( result_t op1, result_t op2, Token tk_ ){
	switch( tk_.value ){
		case '+': return op1 + op2;
		case '-': return op1 - op2;
		case '*': return op1 * op2;
		case '/': if( op2 == 0 )
					{
						curr_status = Evaluator::DIVISION_BY_ZERO;
				  	}
				  return op1 / op2;
		case '%': return op1 % op2;
		case '^': return std::pow( op1, op2 );
		default : assert(false);
	}
}