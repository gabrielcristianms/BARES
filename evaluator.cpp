#include "evaluator.h"
/*!
* \brief compares two operators and return the higher precedence one.
* Compara dois operadores para saber quem é o de maior precedência, muito relevante na hora da conversão de formato infixo para pósfixo
* \param op1 primeiro token que é um operador a ser comparado
* \param op2 segundo token que é um operador a ser comparado
* \return Retorna 'true' caso op1 > op2 ou quando são iguais em precedência e o op1 tenha precedência a direita. Retorna 'false' caso contrário
*/
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
/*!
	\brief verifica se é operador de associação a direita
	Recebe um token e checa se é um operador de associação a direita, que no caso do bares, é somente a potenciação.
	\param tk_ Token a ser avaliado
	\return Retorna 'true' caso seja o operador de potenciação.
*/
bool 
Evaluator::right_association( Token tk_ ){
	return ( tk_.value == "^" );
}
/*!
	\brief Checa se o token é operador
	usando o campo de tipo, podemos verificar se o token em questão é um operador ou não
	\param tk_ Token a ser avaliado
	\return Retorna 'true' caso seja um operador.
*/
bool 
Evaluator::is_operator( Token tk_ ){
	//return ( tk_.value == "+" or tk_.value == "-" or tk_.value == "/" or tk_.value == "%" or tk_.value == "*" or tk_.value == "^");
	return tk_.type == Token::OPERATOR;
}
/*!
	\brief Checa se o token é operando
	usando o campo de tipo, podemos verificar se o token em questão é um operando ou não
	\param tk_ Token a ser avaliado
	\return Retorna 'true' caso seja um operando.
*/
bool 
Evaluator::is_operand( Token tk_ ){
	//return ( tk_.value >= "0" and tk_.value <= "9" );
	return tk_.type == Token::OPERAND;
}
/*!
	\brief Checa se o token é um parentesis abrindo
	Aqui vale mais a pena fazer apenas uma comparação do que usar o campo tipo da estrutura Token, já que ela só diz se é escopo ou não
	\param tk_ Token a ser avaliado
	\return Retorna 'true' caso seja um parentesis abrindo.
*/
bool 
Evaluator::is_opening_scope( Token tk_ ){
	return tk_.value == "(";
}

/*!
	\brief Checa se o token é um parentesis fechando
	Aqui vale mais a pena fazer apenas uma comparação do que usar o campo tipo da estrutura Token, já que ela só diz se é escopo ou não
	\param tk_ Token a ser avaliado
	\return Retorna 'true' caso seja um parentesis fechando.
*/
bool 
Evaluator::is_closing_scope( Token tk_ ){
	return tk_.value == ")";
}
/*!
	\brief verifica qual a precedência de um operador e a retorna.
	Recebe um token e checa qual a precedência dele, associando um número inteiro que é retornado.
	\param tk_ Token a ser avaliado
	\return Retorna um inteiro representativo da precedência dele.
*/
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
/*!
	\brief converte uma expressão de formato infixo para pósfixo.
	Utilizando um vetor de tokens dispostos de maneira infixa, utilizamos uma pilha para fazer a conversão. As funções anteriores de precedência serão
	cruciais nesse ponto
*/

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
/*!
	\brief converte um token operando para um valor inteiro.
	Uma stringstream é utilizada para converter o valor string do token para um valor inteiro, tendo o cuidado com o menos unário
	\param tk_ Token a ser convertido para inteiro
	\return Retorna o valor inteiro da string armazenada no token.
*/
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

/*!
	\brief aplica operações em operandos
	Aqui é onde os cálculos são devidamente feitos, tendo-se o cuidado com divisão por zero.
	\param op1 Primeiro operando
	\param op2 Segundo operando
	\param tk_ Token operador binário que usará os parâmetros op1 e op2.
	\return Retorna o valor inteiro da operação realizada.
*/
Evaluator::result_t
Evaluator::apply_operation( result_t op1, result_t op2, Token tk_ ){
	char ch_ = tk_.value[0];
	switch( ch_ ){
		case '+': return op1 + op2;
		case '-': return op1 - op2;
		case '*': return op1 * op2;
		case '/': if( op2 == 0 )
					{
						curr_status = EvaluatorResult(EvaluatorResult::DIVISION_BY_ZERO);
						return 42; // you'll certainly need a towel now
				  	}
				  return op1 / op2;
		case '%': return op1 % op2;
		case '^': return std::pow( op1, op2 );
		default : assert(false);
	}
}
/*!
	\brief avalia uma expressão pósfixa e retorna o resultado ou um erro.
	Aqui é onde de fato acontecem os diversos cálculos para obtermos o resultado final da expressão passada. Utilizamos uma lista em formato pósfixo
	e jogamos operadores numa pilha até aparecer um operador, desempilha dois operando e retorna o resultado pra pilha. Ao final, irá restar somente
	um elemento na pilha, que será o resultado. A cada operação realizada é checado se o valor está além dos limites de um short int.
	\return Retorna um valor inteiro que é o resultado da expressão que foi avaliada.
*/

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
/*!
	\brief Está é a função que o cliente tem acesso para poder fazer avaliação de expressões
	Recebe-se uma lista de tokens que devem estar dispostos de maneira infixa e as funções de conversão para pósfixo e de avaliação de uma expressão
	pósfixa são chamadas.
	\param e_ lista de tokens em formato infixo.
	\return Retorna um EvaluatorResult, indicando se a avaliação ocorreu sem problemas ou caso contrário qual erro aconteceu.
*/

Evaluator::EvaluatorResult
Evaluator::evaluate( std::vector<Token> e_ ){
	infix_expr = e_; //Guarda a lista de tokens
	curr_status = EvaluatorResult( EvaluatorResult::EVALUATOR_OK ); // "Resetar" a msg de status p/ OK.

    infix_to_postfix();
    final_result = evaluate_postfix();

    return curr_status;
}
/*!
	\brief retorna o valor da expressão avaliada para o cliente.
	\return Retorna um inteiro que é o resultado da expressão passada pro avaliador.
*/

Evaluator::result_t
Evaluator::get_result(void) const{
	return final_result;
}