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
	return ( tk_.value == "^" );
}

bool 
Evaluator::is_operator( Token tk_ ){
	//return ( tk_.value == "+" or tk_.value == "-" or tk_.value == "/" or tk_.value == "%" or tk_.value == "*" or tk_.value == "^");
	return tk_.type == Token::OPERATOR;
}

bool 
Evaluator::is_operand( Token tk_ ){
	//return ( tk_.value >= "0" and tk_.value <= "9" );
	return tk_.type == Token::OPERAND;
}

bool 
Evaluator::is_opening_scope( Token tk_ ){
	return tk_.value == "(";
}


bool 
Evaluator::is_closing_scope( Token tk_ ){
	return tk_.value == ")";
}

int 
Evaluator::get_operator_precedence( Token tk_ ){
	int weight = -1;
	char ch_ = tk_.value[0];

	switch ( ch_ ){
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
Evaluator::infix_to_postfix( void ){
	std::vector< Token > postfix;
	std::stack< Token > S;

	// Percorrer cada caractere da expressão
	for( auto Token : infix_expr ){
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
	postfix_expr = postfix;
}

Evaluator::result_t
Evaluator::tk_2_int( Token tk_ ){
	std::stringstream ss;
	Evaluator::result_t result;

	if( tk_.value[0] == '-' ){
		std::string num;
		for( auto it( tk_.value.begin()+1); it != tk_.value.end(); ++it ) 
			num.push_back( *it );
		ss << num;
		ss >> result;
		result *= -1;
	}
	else{
		ss << tk_.value;
		ss >> result;
	}
	return result;
}

Evaluator::result_t
Evaluator::apply_operation( result_t op1, result_t op2, Token tk_ ){
	char ch_ = tk_.value[0];
	switch( ch_ ){
		case '+': return op1 + op2;
		case '-': return op1 - op2;
		case '*': return op1 * op2;
		case '/': if( op2 == 0 )
					{
						std::cout << "Tou aqui?\n";
						curr_status = EvaluatorResult(EvaluatorResult::DIVISION_BY_ZERO);
						return 42; // you'll certainly need a towel now
				  	}
				  return op1 / op2;
		case '%': return op1 % op2;
		case '^': return std::pow( op1, op2 );
		default : assert(false);
	}
}

Evaluator::result_t
Evaluator::evaluate_postfix( void ){
	std::stack<result_t> S;

	for( auto tk : postfix_expr ){
		if( is_operand( tk ) ){
			S.push( tk_2_int( tk ) );
		}
		else if ( is_operator( tk ) ){
			auto op2 = S.top(); S.pop();
			auto op1 = S.top(); S.pop();

			// Realiza a operação sobre os elementos.
			auto result = apply_operation( op1, op2, tk );
			
			if( (result < -32768) or (result > 32767) ){
    			curr_status = EvaluatorResult( EvaluatorResult::RESULT_OVERFLOW );
    			return 42; // Carry a towel
    		}
			S.push( result );
		}
		else{
			assert( false );
		}
	}
	// A pilha não pode estar vazia, pois no topo deve estar o resultado.
	assert( not S.empty() );
	return S.top();
}

Evaluator::EvaluatorResult
Evaluator::evaluate( std::vector<Token> e_ ){
	infix_expr = e_; //Guarda a lista de tokens
	curr_status = EvaluatorResult( EvaluatorResult::EVALUATOR_OK ); // "Resetar" a msg de status p/ OK.

    infix_to_postfix();
    final_result = evaluate_postfix();

    return curr_status;
}

Evaluator::result_t
Evaluator::get_result(void) const{
	return final_result;
}