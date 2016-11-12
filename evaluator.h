#ifndef _EVALUATOR_H_
#define _EVALUATOR_H_

#include <stack>     // stack
#include <string>    // string
#include <cassert>   // assert
#include <cmath>     // pow

class Evaluator{
	public:
		typedef short int result_t;

		struct EvaluatorResult{
			typedef size_t size_type;

			enum code_t{
					EVALUATOR_OK = 0,
					DIVISION_BY_ZERO,
					RESULT_OVERFLOW
			};

			code_t type;
			size_type at_col;

			explicit EvaluatorResult( code_t type_ = EVALUATOR_OK, size_type col_ = 0u )
				: type{ type_ }
				, at_col{ col_ }
			{/*empty*/}
		};

		EvaluatorResult evaluate( std::vector<Token> e_ );
		result_t get_result() const ;

		/// Constutor default.
        Evaluator() = default;
        ~Evaluator() = default;
        /// Desligar cópia e atribuição.
        Evaluator( const Evaluator & ) = delete;  // Construtor cópia.
        Evaluator & operator=( const Evaluator & ) = delete; // Atribuição.

    private:
    	std::string<Token> infix_expr;
    	std::stack<Token> posfix_expr;

    	/// Converts a expression in infix notation to a corresponding profix representation.
		std::stack<Token> infix_to_postfix( std::string );

		/// Checks whether the first operator has higher precedence over the second one.
		bool has_higher_precedence(Token , Token);

		/// Checks if the token works by right association.
		bool right_association( Token );

		/// Checks whether a token is operator symbol or not. 
		bool is_operator(Token);

		/// Checks whether a token is a character is alphanumeric chanaracter (letter or numeric digit) or not. 
		bool is_operand(Token);

		/// Checks whether the token is an opening scope symbol
		bool is_opening_scope(Token);

		/// Checks whether the token is a closing scope symbol
		bool is_closing_scope(Token);

		/// Returns the precedence of the operator.
		result_t get_operator_precedence(Token);

		/// Return the value of a token.
		result_t char_2_int( Token );

		/// This is where we calculate values and return them.
		result_t apply_operation( result_t op1, result_t op2, Token ch );
};
#endif