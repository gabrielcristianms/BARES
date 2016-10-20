/*!
 * Infix to postfix conversion in C++
 * Input Postfix expression must be in a desired format.
 * Operands and operator, **both must be single character**.
 * Only '+', '-', '*', '/', and '^' (for exponentiation) operators are expected;
 * Any other character is just ignored.
 */

#include <iostream>  // cout, endl
#include <stack>     // stack
#include <string>    // string
#include <cassert>   // assert
#include <cmath>     // pow
#include <stdexcept> // std::runtime_error

/// Converts a expression in infix notation to a corresponding profix representation.
std::string infix_to_postfix( std::string );

/// Checks whether the first operator has higher precedence over the second one.
bool has_higher_precedence(char , char);

bool right_association( char );

/// Checks whether a token is operator symbol or not. 
bool is_operator(char);

/// Checks whether a token is a character is alphanumeric chanaracter (letter or numeric digit) or not. 
bool is_operand(char);

/// Checks whether the token is an opening scope symbol
bool is_opening_scope(char);

/// Checks whether the token is a closing scope symbol
bool is_closing_scope(char);

/// Returns the precedence of the operator.
int get_operator_precedence(char);

bool has_higher_precedence( char op1, char op2 ){
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

bool right_association( char ch_ ){
	return ( ch_ == '^' );
}

bool is_operator( char ch_ ){
	return ( ch_ == '+' or ch_ == '-' or ch_ == '/' or ch_ == '%' or ch_ == '*' or ch_ == '^');
}

bool is_operand( char ch_ ){
	return ( ch_ >= '0' and ch_ <= '9' );
}

bool is_opening_scope( char ch_ ){
	return ch_ == '(';
}


bool is_closing_scope( char ch_ ){
	return ch_ == ')';
}

int get_operator_precedence( char ch_ ){
	int weight = -1;

	switch( ch_ ){
		case '^' : weight = 3; break;
		case '*' : 
		case '/' :
		case '%' : weight = 2; break;
		case '+' :
		case '-' : weight = 1; break;
	}

	return weight;
}

std::string infix_to_postfix( std::string infix ){
	std::string postfix;
	std::stack<char> S;

	// Percorrer cada caractere da expressão
	for( auto ch : infix ){
		// Abertura de escopo.
		if( is_opening_scope( ch ) ){
			S.push( ch );
		}
		else if( is_operand( ch ) ){
			postfix.push_back( ch );
		}
		else if( is_closing_scope( ch ) ){
			// Desempilhar ate achar o escopo de abertura correspondente.
			while( not S.empty() and not is_opening_scope( S.top() ) ){
				postfix.push_back( S.top() );
				S.pop();
			}
			S.pop();
		}
		else if( is_operator( ch ) ){
			// Desempilhar as operações que tem maior (ou igual) prioridade.
			while( not S.empty() and not is_opening_scope( S.top() ) and has_higher_precedence( S.top(), ch ) ){
				postfix.push_back( S.top() );
				S.pop();
			}
			S.push( ch ); // O novo operador sempre entra na pilha.
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

int char_2_int( char ch ){
	return ch - '0';
}

int apply_operation( int op1, int op2, char ch ){
	switch( ch ){
		case '+': return op1 + op2;
		case '-': return op1 - op2;
		case '*': return op1 * op2;
		case '/': if( op2 == 0 ) throw std::runtime_error( "Divisão por zero!" );
				  return op1 / op2;
		case '%': return op1 % op2;
		case '^': return std::pow( op1, op2 );
		default : assert(false);
	}
}

int evaluate_postfix( std::string postfix ){
	std::stack<int> S;

	for( auto ch : postfix ){
		if( is_operand( ch ) ){
			S.push( char_2_int( ch ) );
		}
		else if ( is_operator( ch ) ){
			auto op2 = S.top(); S.pop();
			auto op1 = S.top(); S.pop();

			// Realiza a operação sobre os elementos.
			auto result = apply_operation( op1, op2, ch );
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

int main()
{
    //std::string expression = "4/(5^2)+(6^2^3)";
    std::string expression = "1+ 3 * ( 4 + 8 * 3 ^ 7)";
    //std::string expression = "A+(B*C-(D/E^F)+G)*H";
    auto postfix = infix_to_postfix(expression);
    std::cout << ">>> Input (infix)    = " << expression << "\n";
    std::cout << ">>> Output (postfix) = " << postfix << "\n";

    auto result = evaluate_postfix( postfix );
    std::cout << ">>> Result is: " << result << std::endl;

    std::cout << "\n>>> Normal exiting...\n";

    return EXIT_SUCCESS;
}

