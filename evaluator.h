#ifndef _EVALUATOR_H_
#define _EVALUATOR_H_

class Evaluator{
	public:
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
		//TODO resto da classe evaluate

		/// Constutor default.
        Parser() = default;
        ~Parser() = default;
        /// Desligar cópia e atribuição.
        Parser( const Parser & ) = delete;  // Construtor cópia.
        Parser & operator=( const Parser & ) = delete; // Atribuição.
};

#endif